#ifndef UTILS_H
#define UTILS_H

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

int GetDictionaryValueAsInt(CFDictionaryRef window, CFStringRef prop);
void PrintProperties(AXUIElementRef windowRef);

#endif // UTILS_H
