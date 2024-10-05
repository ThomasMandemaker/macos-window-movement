
#ifndef VIEW_H
#define VIEW_H

#include "CMacsTypes.h"
#include <CoreFoundation/CoreFoundation.h>

void View_drawRect(id self, SEL _cmd, CGRect rect);

extern void NSRectFill(CGRect aRect);
extern void NSFrameRect(CGRect aRect);

#endif // !VIEW_H
