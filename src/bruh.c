#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "config.h"
#include "layout.h"
#include "window.h"

size_t mainDisplayHeight;
size_t mainDisplayWidth;
Layout* layout = NULL;


void callback(int64_t key) {
    if (key == 12) {
        ResizeFocusedWindow(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0)); // Reset to fullscreen
        release();
        DestroyLayout(layout);
        exit(0);
    }

    Panel* nextPanel = NextPanel(layout, key);

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
    layout = Load();

    if (!layout) {
        layout = InitLayout(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0));
    }

    init_listener(callback);

}

