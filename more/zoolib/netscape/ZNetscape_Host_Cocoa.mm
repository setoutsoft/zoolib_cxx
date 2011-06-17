/* -------------------------------------------------------------------------------------------------
Copyright (c) 2010 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------- */

#include "zoolib/netscape/ZNetscape_Host_Cocoa.h"

#if ZCONFIG_SPI_Enabled(Cocoa)

#include <AppKit/NSEvent.h>
#include <AppKit/NSGraphicsContext.h>

#include "zoolib/ZLog.h"
#include "zoolib/ZMemory.h"
#include "zoolib/ZRGBA.h"
#include "zoolib/ZStream_String.h"
#include "zoolib/ZUtil_Strim_Geom.h"
#include "zoolib/ZWorker.h"
#include "zoolib/ZWorkerRunner_CFRunLoop.h"

// =================================================================================================
#pragma mark -
#pragma mark * NSView_NetscapeHost

@implementation NSView_NetscapeHost

- (id)init
	{
	[super init];
	fHost = nullptr;

	NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:self
		selector:@selector(frameChanged:)
		name:NSViewFrameDidChangeNotification
		object:self];

	return self;
	}

- (void)dealloc
	{
	NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
	[nc removeObserver:self]; 

	if (fHost)
		{
		delete fHost;
		fHost = nullptr;
		}
	[super dealloc];
	}

- (BOOL)isFlipped
	{ return YES; }

- (BOOL)acceptsFirstResponder
	{ return YES; }

- (BOOL)becomeFirstResponder
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->FocusChanged(true);
	return YES;
	}

- (BOOL)resignFirstResponder
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->FocusChanged(false);
	return YES;
	}

- (void)mouseDown:(NSEvent *)theEvent
	{
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseDown);
	}

- (void)mouseUp:(NSEvent *)theEvent
	{
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseUp);
	}

- (void)mouseMoved:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug + 1);
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseMoved);
	}

- (void)mouseDragged:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseDragged);
	}

- (void)mouseEntered:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseEntered);
	}

- (void)mouseExited:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->SendMouseEvent(theEvent, NPCocoaEventMouseExited);
	}

- (void)keyDown:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		{
		fHost->SendKeyEvent(theEvent, NPCocoaEventKeyDown);
		}
	}

- (void)keyUp:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->SendKeyEvent(theEvent, NPCocoaEventKeyUp);
	}

- (void)flagsChanged:(NSEvent *)theEvent
	{
	ZLOGTRACE(eDebug);
	if (fHost)
		fHost->FlagsChanged(theEvent);
	}

- (void)drawRect:(NSRect)dirtyRect
	{
	if (fHost)
		fHost->Draw(dirtyRect);
	}

- (void)frameChanged:(id)sender
	{
	if (fHost)
		fHost->FrameChanged();
	}

@end

// =================================================================================================
#pragma mark -
#pragma mark * Host_Cocoa

namespace ZooLib {
namespace ZNetscape {

class Host_Cocoa::Worker_Timer : public ZWorker
	{
public:
	Worker_Timer(Host_Cocoa* iHost)
	:	fHost(iHost)
		{}

	virtual bool Work()
		{
		if (fHost)
			{
			fHost->Timer();
			this->WakeIn(50e-3); // 50ms
			return true;
			}
		return false;
		}

	virtual void Kill()
		{
		fHost = nullptr;
		}

	Host_Cocoa* fHost;
	};

Host_Cocoa::Host_Cocoa(ZRef<GuestFactory> iGuestFactory, NSView_NetscapeHost* iView)
:	Host_Std(iGuestFactory)
,	fView(iView)
	{
	ZMemZero_T(fNP_CGContext);
	ZMemZero_T(fNP_CGContext_Prior);
	if (fView)
		fView->fHost = this;

	fWorker_Timer = new Worker_Timer(this);
	ZWorkerRunner_CFRunLoop::sMain()->Attach(fWorker_Timer);
	fWorker_Timer->Wake();

//	fNPWindow.type = NPWindowTypeWindow;
	fNPWindow.type = NPWindowTypeDrawable;//NPWindowTypeWindow;
	fNPWindow.window = &fNP_CGContext;
	fNPWindow.x = 0;
	fNPWindow.y = 0;
	fNPWindow.width = 400;
	fNPWindow.height = 300;
	fNPWindow.clipRect.left = 0;
	fNPWindow.clipRect.top = 0;
	fNPWindow.clipRect.right = 400;
	fNPWindow.clipRect.bottom = 300;
	}

Host_Cocoa::~Host_Cocoa()
	{
	if (ZRef<ZWorker> theWorker = fWorker_Timer)
		{
		fWorker_Timer.Clear();
		theWorker->Kill();
		}

	if (fView)
		ZAssert(fView->fHost == this);
	}

NPError Host_Cocoa::Host_GetValue(NPP npp, NPNVariable variable, void* ret_value)
	{
	switch (variable)
		{
		case NPNVsupportsCoreAnimationBool:
			{
			*static_cast<NPBool*>(ret_value) = false;
			return NPERR_NO_ERROR;
			}
		case NPNVsupportsCoreGraphicsBool:
			{
			*static_cast<NPBool*>(ret_value) = true;
			return NPERR_NO_ERROR;
			}
		case NPNVsupportsCocoaBool:
			{
			*static_cast<NPBool*>(ret_value) = true;
			return NPERR_NO_ERROR;
			}
		case NPNVSupportsWindowless:
			{
			*static_cast<NPBool*>(ret_value) = false;
			return NPERR_NO_ERROR;
			}
		case NPNVprivateModeBool:
			{
			*static_cast<NPBool*>(ret_value) = false;
			return NPERR_NO_ERROR;
			}
		}

	return Host_Std::Host_GetValue(npp, variable, ret_value);
	}

NPError Host_Cocoa::Host_SetValue(NPP npp, NPPVariable variable, void* value)
	{
	switch (variable)
		{
		case NPPVpluginDrawingModel:
			{
			ZAssert(reinterpret_cast<intptr_t>(value) == NPDrawingModelCoreGraphics);
			return NPERR_NO_ERROR;
			}
		case NPPVpluginTransparentBool:
			{
			return NPERR_NO_ERROR;
			}
		case NPPVpluginEventModel:
			{
			ZAssert(reinterpret_cast<intptr_t>(value) == NPEventModelCocoa);
			return NPERR_NO_ERROR;
			}
		}

	return Host_Std::Host_SetValue(npp, variable, value);
	}

void Host_Cocoa::Host_InvalidateRect(NPP npp, NPRect* rect)
	{
	NSRect theBounds =
		NSMakeRect(rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top);
	[fView setNeedsDisplayInRect:theBounds];
	}

void Host_Cocoa::PostCreateAndLoad()
	{
	Host_Std::PostCreateAndLoad();
	this->pDoSetWindow();
	[fView setNeedsDisplay:true];
	this->FocusChanged(true);
	}

void Host_Cocoa::pDoSetWindow()
	{
	ZGRectf theFrame = [fView frame];
	float width = theFrame.Width();
	float height = theFrame.Height();

	fNPWindow.width = width;
	fNPWindow.height = height;
	fNPWindow.clipRect.left = 0;
	fNPWindow.clipRect.top = 0;
	fNPWindow.clipRect.right = width;
	fNPWindow.clipRect.bottom = height;

	this->Guest_SetWindow(&fNPWindow);
	}

static inline void spInitialize(NPCocoaEvent& oEvent, NPCocoaEventType iType)
	{
	ZMemZero_T(oEvent);
	oEvent.type = iType;
	oEvent.version = 0;
	}

void Host_Cocoa::Draw(const NSRect& iRect)
	{
	CGContextRef context = static_cast<CGContextRef>([[NSGraphicsContext currentContext] graphicsPort]);
	fNP_CGContext.context = context;

	this->Guest_SetWindow(&fNPWindow);

	NPCocoaEvent event;
	spInitialize(event, NPCocoaEventDrawRect);

	event.data.draw.context = context;
	event.data.draw.x = iRect.origin.x;
	event.data.draw.y = iRect.origin.y;
	event.data.draw.width = iRect.size.width;
	event.data.draw.height = iRect.size.height;

	this->Guest_HandleEvent(&event);
	}

void Host_Cocoa::FlagsChanged(NSEvent* iEvent)
	{
	NPCocoaEvent event;
	spInitialize(event, NPCocoaEventFlagsChanged);

	event.data.key.modifierFlags = [iEvent modifierFlags];
	event.data.key.keyCode = [iEvent keyCode];
	event.data.key.isARepeat = false;
	event.data.key.characters = 0;
	event.data.key.charactersIgnoringModifiers = 0;

	this->Guest_HandleEvent(&event);
	}

void Host_Cocoa::SendText(NSEvent* iEvent)
	{
	NPCocoaEvent event;
	spInitialize(event, NPCocoaEventTextInput);
	event.data.text.text = (NPNSString *)[iEvent characters];
 
	this->Guest_HandleEvent(&event);
	}

bool Host_Cocoa::SendKeyEvent(NSEvent* iEvent, NPCocoaEventType iType)
	{
	NPCocoaEvent event;
	spInitialize(event, iType);

	event.data.key.modifierFlags = [iEvent modifierFlags];
	event.data.key.keyCode = [iEvent keyCode];
	event.data.key.isARepeat = [iEvent isARepeat];
	event.data.key.characters = (NPNSString*)[iEvent characters];
	event.data.key.charactersIgnoringModifiers = (NPNSString*)[iEvent charactersIgnoringModifiers];

	int result = this->Guest_HandleEvent(&event);

	ZAssert(result == 0 || result == 1);

	return result != 0;
	}

bool Host_Cocoa::SendMouseEvent(NSEvent* iEvent, NPCocoaEventType iType)
	{
	NPCocoaEvent event;
	spInitialize(event, iType);

	NSPoint point = [fView convertPoint:[iEvent locationInWindow] fromView:nil];

	int clickCount;
	if (iType == NPCocoaEventMouseEntered
		|| iType == NPCocoaEventMouseExited
		|| iType == NPCocoaEventScrollWheel)
		{
		clickCount = 0;
		}
	else
		{
		clickCount = [iEvent clickCount];
		}

	event.data.mouse.modifierFlags = [iEvent modifierFlags];
	event.data.mouse.buttonNumber = [iEvent buttonNumber];
	event.data.mouse.clickCount = clickCount;
	event.data.mouse.pluginX = point.x;
	event.data.mouse.pluginY = point.y;
	event.data.mouse.deltaX = [iEvent deltaX];
	event.data.mouse.deltaY = [iEvent deltaY];
	event.data.mouse.deltaZ = [iEvent deltaZ];

	return this->Guest_HandleEvent(&event);
	}

void Host_Cocoa::FocusChanged(bool hasFocus)
	{
	NPCocoaEvent event;
	spInitialize(event, NPCocoaEventFocusChanged);
	event.data.focus.hasFocus = hasFocus;
	this->Guest_HandleEvent(&event);
	}

void Host_Cocoa::WindowFocusChanged(bool hasFocus)
	{
	NPCocoaEvent event;
	spInitialize(event, NPCocoaEventWindowFocusChanged);
	event.data.focus.hasFocus = hasFocus;
	this->Guest_HandleEvent(&event);
	}

void Host_Cocoa::Timer()
	{
	Host_Std::DeliverData();
	}

void Host_Cocoa::FrameChanged()
	{
	ZLOGTRACE(eDebug);
	this->pDoSetWindow();
	}

} // namespace ZNetscape
} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(Cocoa)
