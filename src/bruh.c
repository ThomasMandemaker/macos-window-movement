#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "keyboard.h"
#include "window.h"

const int64_t KEYUP = 126;
const int64_t KEYRIGHT = 124;
const int64_t KEYDOWN = 125;
const int64_t KEYLEFT = 123;

size_t mainDisplayHeight;
size_t mainDisplayWidth;

int GetDictionaryValueAsInt(CFDictionaryRef window, CFStringRef prop) {
    int value;
    CFNumberRef propertyValue = CFDictionaryGetValue(window, prop);
    CFNumberGetValue(propertyValue, kCFNumberIntType, &value);
    return value;
}

void PrintProperties(AXUIElementRef windowRef) {
    CFArrayRef arr;
    AXError result = AXUIElementCopyAttributeNames(windowRef, &arr);

    long count = (long)CFArrayGetCount(arr);
    for (long i = 0; i < count; i++) {
        CFStringRef attribute = CFArrayGetValueAtIndex(arr, i);

        CFIndex length = CFStringGetLength(attribute);
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        char *buffer = (char *)malloc(maxSize);
        
        if (!CFStringGetCString(attribute, buffer, maxSize,
                    kCFStringEncodingUTF8)) {
            printf("Failed");
            free(buffer);
        }

        printf("Text: %s \n", buffer);
        free(buffer);
    }
}

void Callback(int64_t key) {
    switch (key) {
        case KEYUP: ResizeFocusedWindow(mainDisplayHeight/2, mainDisplayWidth, 0, 0); break; 
        case KEYRIGHT: ResizeFocusedWindow(mainDisplayHeight, mainDisplayWidth/2, mainDisplayWidth/2, 0); break; 
        case KEYDOWN: ResizeFocusedWindow(mainDisplayHeight/2, mainDisplayWidth, 0, mainDisplayHeight/2); break; 
        case KEYLEFT: ResizeFocusedWindow(mainDisplayHeight, mainDisplayWidth/2, 0, 0); break; 
        default: break;
    }
}


int main() {
    if (!AXIsProcessTrusted()) {
        printf("Process is not trusted, exiting...");
        return -1;
    }

    
    mainDisplayHeight = CGDisplayPixelsHigh(CGMainDisplayID());
    mainDisplayWidth = CGDisplayPixelsWide(CGMainDisplayID());
    init_listener(Callback);

    Release();
}

