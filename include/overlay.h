#ifndef OVERLAY_H
#define OVERLAY_H

#include <CoreFoundation/CoreFoundation.h>
#include <stdlib.h>
#include <stdio.h>
#include "CMacsTypes.h"

void CreateAppOverlay(size_t width, size_t height);
void RunApplication(void);
void ResizeView(CGRect rect);
void AddWindow(CGRect rect);

#endif // !OVERLAY_H
