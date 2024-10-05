#ifndef UTILS_H
#define UTILS_H

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include "layout.h"

int GetDictionaryValueAsInt(CFDictionaryRef window, CFStringRef prop);
void PrintProperties(AXUIElementRef windowRef);
void PrintPanelProperties(char* prefix, Panel* panel);
void Print(char* text);

#endif // UTILS_H
