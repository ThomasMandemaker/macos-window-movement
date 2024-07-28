#include "window.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

AXUIElementRef systemWideElement = NULL;
CFArrayRef windows = NULL;

void ResizeFocusedWindow(int height, int width, int x, int y) {
    if (!windows) {
        windows = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
    }

    if (!systemWideElement) {
        systemWideElement = AXUIElementCreateSystemWide();
    }

    AXUIElementRef focusedApp;
    AXError err = AXUIElementCopyAttributeValue(systemWideElement, kAXFocusedApplicationAttribute, (CFTypeRef* ) &focusedApp);
    if (err) {
        printf("Error copying focused appication, err: %d\n", err);
        return;
    }

    AXUIElementRef window;
    err = AXUIElementCopyAttributeValue(focusedApp, kAXFocusedWindowAttribute, (CFTypeRef* ) &window);
    if (err) {
        CFRelease(focusedApp);
        printf("Error copying focused window, err: %d\n", err);
        return;
    }
    
    CGSize size;
    CGPoint point;
    size.width = width;
    size.height = height;
    point.x = x;
    point.y = y;

    AXValueRef temp = AXValueCreate( kAXValueCGSizeType, &size );
    AXUIElementSetAttributeValue(window, kAXSizeAttribute, temp );
    temp = AXValueCreate( kAXValueCGPointType, &point );
    AXUIElementSetAttributeValue(window, kAXPositionAttribute, temp );

    CFRelease(temp);
    CFRelease(window);
    CFRelease(focusedApp);
}

void release() {
    if (!systemWideElement) {
        return;
    }

    CFRelease(systemWideElement);
    CFRelease(windows);
}
