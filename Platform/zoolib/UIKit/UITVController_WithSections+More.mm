/* -------------------------------------------------------------------------------------------------
Copyright (c) 2014 Andrew Green
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

#include "zoolib/UIKit/UITVController_WithSections+More.h"

#if ZCONFIG_SPI_Enabled(iPhone)

#include "zoolib/Apple/Callable_ObjC.h"
#include "zoolib/Apple/Cartesian_CG.h"
#include "zoolib/Apple/Util_NS.h"

#import <UIKit/UIKit.h>

using namespace ZooLib;

// =================================================================================================
#pragma mark -
#pragma mark ZooLib::UIKit

namespace ZooLib {
namespace UIKit {

ZRef<Section> sMakeSection(ZRef<SectionBody> iBody, bool iHideWhenEmpty)
	{
	ZRef<Section> theSection = new Section(iBody);
	theSection->fHideWhenEmpty = iHideWhenEmpty;
	if (iHideWhenEmpty)
		{
		theSection->fSectionAnimation_InsertQ = UITableViewRowAnimationFade;
		theSection->fSectionAnimation_DeleteQ = UITableViewRowAnimationFade;
		}
	return theSection;
	}

ZRef<Section> sMakeSection(ZRef<SectionBody> iBody)
	{ return sMakeSection(iBody, false); }

ZRef<SectionBody_SingleRow> sMakeSingleRow(ZRef<SectionBody_Multi> iParent,
	ZRef<SectionBody::Callable_RowSelected> iCallable_RowSelected,
	ZRef<SectionBody::Callable_ButtonTapped> iCallable_ButtonTapped)
	{
	ZRef<SectionBody_SingleRow> theSB = new SectionBody_SingleRow(null);
	if (iParent)
		iParent->fBodies_Pending.push_back(theSB);
	theSB->fShouldIndentWhileEditingQ = false;
	theSB->fCallable_RowSelected_Editing = iCallable_RowSelected;
	theSB->fCallable_ButtonTapped = iCallable_ButtonTapped;
	theSB->fRowAnimation_Insert = UITableViewRowAnimationNone;
	theSB->fRowAnimation_Delete = UITableViewRowAnimationNone;
	theSB->fRowAnimation_Reload = UITableViewRowAnimationFade;//##
	return theSB;
	}

ZRef<SectionBody_SingleRow> sMakeSingleRow(ZRef<SectionBody_Multi> iParent,
	id iDelegate, SEL iSEL)
	{
	return sMakeSingleRow(iParent,
		sCallable<SectionBody::Callable_RowSelected::Signature>(iDelegate, iSEL),
		null);
	}

ZRef<UILabel> sMakeUILabel()
	{
	ZRef<UILabel> theLabel = sAdopt& [[UILabel alloc] initWithFrame:CGRectZero];
	[theLabel setHighlightedTextColor:[UIColor whiteColor]];
	[theLabel setBackgroundColor:[UIColor clearColor]];
	[theLabel setOpaque:NO];
	return theLabel;
	}

} // namespace UIKit
} // namespace ZooLib

#endif // ZCONFIG_SPI_Enabled(iPhone)
