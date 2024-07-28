#include "utils.h"

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
