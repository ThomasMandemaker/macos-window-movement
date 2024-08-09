#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "layout.h"
#include "window.h"

size_t mainDisplayHeight;
size_t mainDisplayWidth;


void callback(int64_t key) {
    if (key == 12) {
        ResizeFocusedWindow(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0)); // Reset to fullscreen
        release();
        FreeAll();
        exit(0);
    }

    Panel* nextPanel = NextPanel(key);

    if (nextPanel == NULL) {
        return;
    }

    ResizeFocusedWindow(nextPanel->size, nextPanel->point);
}


int main() {
    if (!AXIsProcessTrusted()) {
        printf("Process is not trusted, exiting...");
        return -1;
    }

    
    mainDisplayHeight = CGDisplayPixelsHigh(CGMainDisplayID());
    mainDisplayWidth = CGDisplayPixelsWide(CGMainDisplayID());
    Panel* originalPanel = InitLayout(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0));
    // Maybe we can save them using commands, rather than data?
    SplitHorizontal(originalPanel);
    SplitVertical(originalPanel);
    SplitVertical(originalPanel);
    SplitHorizontal(originalPanel);
    SplitVertical(originalPanel->right);
    RemovePanel(originalPanel->right->down);
    RemovePanel(originalPanel->down);

    init_listener(callback);

}

