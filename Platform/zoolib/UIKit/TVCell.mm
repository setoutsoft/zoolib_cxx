#include "zoolib/UIKit/TVCell.h"
#include <UIKit/UILabel.h>

// =================================================================================================
#pragma mark -

namespace ZooLib {
namespace UIKit {

ZRef<UITableViewCell> sGetCell_Simple(UITableView* iView, NSString* iText, bool iReusable)
	{
	ZRef<UITableViewCell> theCell = sGetCell_T<UITableViewCell>(iView, iReusable);
	[theCell textLabel].text = iText;
	return theCell;
	}

} // namespace UIKit
} // namespace ZooLib
