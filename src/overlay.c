
#include "overlay.h"
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>

/// A reference to NSApp.  Always a good idea, seeing has he's probably the most helpful thing in CocoaLand
extern id NSApp;

/// This is a strong reference to the class of the AppDelegate
/// (same as [AppDelegate class])
Class AppDelClass;

id window;

size_t mainDisplayHeight;
size_t mainDisplayWidth;

BOOL AppDel_didFinishLaunching(AppDelegate *self, SEL _cmd, id notification) {
	self->window = cmacs_simple_msgSend((id)objc_getClass("NSWindow"), sel_getUid("alloc"));
	
	/// Create an instance of the window.
	self->window = cmacs_window_init_msgSend(self->window, sel_getUid("initWithContentRect:styleMask:backing:defer:"), (CGRect){0,0,mainDisplayWidth,mainDisplayHeight}, (NSClosableWindowMask | NSBorderlessWindowMask | NSMiniaturizableWindowMask), 0, false);

    window = self->window;
    printf("TEST");
	
	/// Create an instance of our view class.
	/// Relies on the view having declared a constructor that allocates a class pair for it.
	id mainView = cmacs_rect_msgSend1(cmacs_simple_msgSend((id)objc_getClass("NSView"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), (CGRect){0,0,mainDisplayWidth,mainDisplayHeight});
    id backgroundColor = cmacs_color_msgSend((id)objc_getClass("NSColor"), sel_getUid("colorWithSRGBRed:green:blue:alpha:"), 0.0, 0.0, 0.0, 0.5);

	cmacs_void_msgSend1(mainView, sel_getUid("setBackgroundColor:"), backgroundColor);

	id view = cmacs_rect_msgSend1(cmacs_simple_msgSend((id)objc_getClass("View"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), (CGRect){ 0, 0, 10, 10 });

    cmacs_void_msgSend1(mainView, sel_getUid("addSubview:"), view);

	// here we simply add the view to the window.
	cmacs_void_msgSend1(self->window, sel_getUid("setContentView:"), mainView);
	cmacs_simple_msgSend(self->window, sel_getUid("becomeFirstResponder"));
	
	// Shows our window in the bottom-left hand corner of the screen.
	cmacs_void_msgSend1(self->window, sel_getUid("makeKeyAndOrderFront:"), self);
	cmacs_void_msgSend1(self->window, sel_getUid("setOpaque:"), NO);

	id clear = cmacs_simple_msgSend((id)objc_getClass("NSColor"), sel_getUid("clearColor"));
	cmacs_void_msgSend1(self->window, sel_getUid("setBackgroundColor:"), clear);

	return YES;
}

void CreateAppOverlay(size_t width, size_t height) {
    mainDisplayWidth = width;
    mainDisplayHeight = height;
	AppDelClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "AppDelegate", 0);
	class_addMethod(AppDelClass, sel_getUid("applicationDidFinishLaunching:"), (IMP)AppDel_didFinishLaunching, "i@:@");
	objc_registerClassPair(AppDelClass);
}

void RunApplication(void) {
	cmacs_simple_msgSend((id)objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
	
	if (NSApp == NULL) {
		fprintf(stderr,"Failed to initialized NSApplication...  terminating...\n");
		return;
	}
	
	id appDelObj = cmacs_simple_msgSend((id)objc_getClass("AppDelegate"), sel_getUid("alloc"));
	appDelObj = cmacs_simple_msgSend(appDelObj, sel_getUid("init"));
	
	cmacs_void_msgSend1(NSApp, sel_getUid("setDelegate:"), appDelObj);
	cmacs_void_msgSend(NSApp, sel_getUid("run"));
}

id test;

void ResizeView(CGRect rect) {
    cmacs_rect_msgSend1(test, sel_getUid("setFrame:"), rect);
}

void AddWindow(CGRect rect) {
    id contentView = cmacs_simple_msgSend(window, sel_getUid("contentView"));
    
	test = cmacs_rect_msgSend1(cmacs_simple_msgSend((id)objc_getClass("View"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), rect);

    cmacs_void_msgSend1(contentView, sel_getUid("addSubview:"), test);
}
