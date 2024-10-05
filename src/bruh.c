#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ApplicationServices/ApplicationServices.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "keyboard.h"
#include "layout.h"
#include "overlay.h"
#include "window.h"

size_t mainDisplayHeight;
size_t mainDisplayWidth;
Layout* layout = NULL;

enum Mode {
    Run,
    Edit
};


/// Hit option-c for config mode
/// A layout might have a draw function?
/// Option-s to split?
/// Ability to select a layout
/// When selected, becomes red, otherwise, some other color
/// When multiple selected, option-m to merge into one (except if it doesn't form a square)
/// Someway to undo the previous action
///
///
/// Hit enter to confirm and switch back to regular mode



void KeyboardCallback(int64_t key) {
    
    if (key == 14) {
        ResizeView((CGRect) {0, 0, 100, 100} );
    }

    if (key == 13) {
        AddWindow((CGRect) {layout->currentPanel->point.x,layout->currentPanel->point.y,layout->currentPanel->size.width, layout->currentPanel->size.height});
        return;
//        ResizeFocusedWindow(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0)); // Reset to fullscreen
//        return;
    }

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


//int main() {
//    if (!AXIsProcessTrusted()) {
//        printf("Process is not trusted, exiting...");
//        return -1;
//    }
//
//    mainDisplayHeight = CGDisplayPixelsHigh(CGMainDisplayID());
//    mainDisplayWidth = CGDisplayPixelsWide(CGMainDisplayID());
//
//    layout = InitLayout(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0));
//    SplitHorizontal(layout, layout->currentPanel);
//
//    /// TODO: ADD A WAY TO RESIZE CONSOLE BY STEPS OF 10 OR SOMTHING BY ARROWS
//    init_listener(KeyboardCallback);
//    CreateAppOverlay(mainDisplayWidth, mainDisplayHeight);
//    RunApplication();
//    return EXIT_SUCCESS;
//}



//int main() {
//
//    mainDisplayHeight = CGDisplayPixelsHigh(CGMainDisplayID());
//    mainDisplayWidth = CGDisplayPixelsWide(CGMainDisplayID());
//
//    layout = InitLayout(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0));
//    SplitHorizontal(layout, layout->currentPanel);
////    SplitHorizontal(layout, layout->currentPanel->right);
////    layout->currentPanel->size.width = mainDisplayWidth/3;
////
////
////    layout->currentPanel->right->size.width = mainDisplayWidth/3;
////    layout->currentPanel->right->point.x = mainDisplayWidth/3;
////
////    layout->currentPanel->right->right->size.width = mainDisplayWidth/3;
////    layout->currentPanel->right->right->point.x = mainDisplayWidth/3 *2;
//
//    init_listener(callback);
////    layout = Load();
////
////    if (!layout) {
////        layout = InitLayout(CGSizeMake(mainDisplayWidth, mainDisplayHeight), CGPointMake(0, 0));
////    }
//
////    CGContextRef c = CGBitmapContextCreate(NULL, mainDisplayWidth, mainDisplayHeight, 8, 4*mainDisplayWidth, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaNone);
////
////    CGDirectDisplayID displayID = CGMainDisplayID();
////    CGRect displayBounds = CGRectMake(0, 0, mainDisplayWidth, mainDisplayHeight);
////    CGWindowID windowID;
////    CGWindowListCreateImage(displayBounds, kCGWindowListOptionOnScreenOnly, kCGWindowListOptionOnScreenOnly, kCGWindowImageBoundsIgnoreFraming);
//    
//    // Draw the square
////    drawSquare(c);
//
////    CGImageRef image = CGBitmapContextCreateImage(c);
//    
//    // Draw the image to the screen
////    CGContextRef screenContext = CGDisplayGetDrawingContext(displayID);
////    CGColorRef color= CGColorCreateGenericRGB(0.2, 0.5, 0.0, 1.0);
////    CGContextSetFillColorWithColor(screenContext, color);
////    CGContextDrawImage(screenContext, displayBounds, image);
////    CGContextFlush(screenContext);
//    
//    // Release resources
////    CGImageRelease(image);
////    CGContextRelease(c);
//
//}

