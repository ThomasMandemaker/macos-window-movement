//
//  ViewController.c
//  CBasediOSApp
//
//  Created by Richard Ross on 4/23/12.
//  Copyright (c) 2012 Ultimate Computer Services, Inc. All rights reserved.
//

#include "View.h"
#include "direction.h"
#include <CoreFoundation/CoreFoundation.h>
#include <stdio.h>


// This is a strong reference to the class of our custom view,
// In case we need it in the future.
Class ViewClass;
Class WindowClass;

bool down = false;
bool dragging = false;
bool grabbing = false;

CGRect currentSize;
CGRect updatedSize;

typedef enum {
    SIDE_TOP,
    SIDE_RIGHT,
    SIDE_BOTTOM,
    SIDE_LEFT,
    SIDE_NONE
} Side;

typedef CGRect (*SideHandler)(CGPoint point, CGRect current);

CGRect handleTop(CGPoint point, CGRect current) {
    current.size.height = current.origin.y + point.y;
    return current;
}

CGRect handleRight(CGPoint point, CGRect current) {
    current.size.width = point.x - current.origin.x;
    return current;
}

CGRect handleDown(CGPoint point, CGRect current) {
    current.origin.y = point.y;
    current.size.height = current.size.height - current.origin.y;
    return current;
}

CGRect handleLeft(CGPoint point, CGRect current) {
    float delta = point.x - current.origin.x;
    current.origin.x = point.x;
    current.size.width -= delta;
//    current.origin.x = point.x;
//    printf("Point x: %f\n",point.x);
//    printf("Origin x: %f\n",current.origin.x);
//    current.origin.x = point.x;
    
//    current.size.width = current.size.width - current.origin.x;
    
//
//    float delta = current.origin.x - point.x;
//    printf("Delta: %f\n",delta);
//    printf("Point x: %f\n",point.x);
//    printf("Point x: %f\n",current.origin.x);
//    current.origin.x -= delta;
//    current.size.width = current.size.width + delta;
    return current;
}

SideHandler sideHandlers[] = {
    handleTop,
    handleRight,
    handleDown,
    handleLeft
};

Side determineSide(CGPoint point, CGRect currentSize) {
    if (point.x < currentSize.origin.x + currentSize.size.width + 10 && point.x > currentSize.origin.x + currentSize.size.width -10) {
        return SIDE_RIGHT;
    } else if (point.x < currentSize.origin.x + 10 && point.x > currentSize.origin.x - 10) {
        return SIDE_LEFT;
    } else if (point.y < currentSize.origin.y + currentSize.size.height + 10 && point.y > currentSize.origin.y + currentSize.size.height -10) {
        return SIDE_TOP;
    } else if (point.y < currentSize.origin.y + 10 && point.y > currentSize.origin.y - 10) {
        return SIDE_BOTTOM;
    }

    return SIDE_NONE;
}

// TODO: Add reference to a layout here.
// TODO: Set like a selected side

void View_MouseDown(id self, SEL _cmd, void * event) {
    down = true;
}

void View_MouseDragged(id self, SEL _cmd, void * event) {
    if (down == true) {

        CGPoint point = cmacs_point_msgSend1((id)objc_getClass("NSEvent"), sel_getUid("mouseLocation"));
//        CGPoint point = cmacs_point_msgSend1(event, sel_getUid("locationInWindow"));

        if (dragging && !grabbing) {
            return;
        }

        Side side = determineSide(point, currentSize);

        if (side == SIDE_NONE) {
            dragging = true;
            return;
        }

        grabbing = true;

        CGRect newSize = sideHandlers[side](point, currentSize);

        cmacs_rect_msgSend1(self, sel_getUid("setFrame:"), newSize);
        cmacs_rect_msgSend1(self, sel_getUid("setBounds:"), newSize);
    }
    dragging = true;
}

void View_MouseUp(id self, SEL _cmd, void * event) {
    down = false;
    dragging = false;
    grabbing = false;
}


void View_drawRect(id self, SEL _cmd, CGRect rect) {
	//make a red cmacs_simple_msgSend object with its convenience method
	id red = cmacs_simple_msgSend((id)objc_getClass("NSColor"), sel_getUid("redColor"));
	
    currentSize = cmacs_rect(self, sel_getUid("frame"));
    CGRect bounds = cmacs_rect(self, sel_getUid("bounds"));
    printf("currentSize x: %f\n", currentSize.origin.x);
    printf("Bounds x: %f\n", bounds.origin.x);

	cmacs_simple_msgSend(red, sel_getUid("set"));
	NSFrameRect ( currentSize );
}

// Once again we use the (constructor) attribute. generally speaking,
// having many of these is a very bad idea, but in a small application
// like this, it really shouldn't be that big of an issue.
__attribute__((constructor))
static void initView() {
	
	// Once again, just like the app delegate, we tell the runtime to
	// create a new class, this time a subclass of 'UIView' and named 'View'.
	ViewClass = objc_allocateClassPair((Class)objc_getClass("NSView"), "View", 0);
	
	// and again, we tell the runtime to add a function called -drawRect:
	// to our custom view. Note that there is an error in the type-specification
	// of this method, as I do not know the @encode sequence of 'CGRect' off
	// of the top of my head. As a result, there is a chance that the rect
	// parameter of the method may not get passed properly.
	class_addMethod(ViewClass, sel_getUid("drawRect:"), (IMP) View_drawRect, "v@:");
	class_addMethod(ViewClass, sel_getUid("mouseDown:"), (IMP) View_MouseDown, "v@:");
	class_addMethod(ViewClass, sel_getUid("mouseDragged:"), (IMP) View_MouseDragged, "v@:");
	class_addMethod(ViewClass, sel_getUid("mouseUp:"), (IMP) View_MouseUp, "v@:");
	
	// And again, we tell the runtime that this class is now valid to be used.
	// At this point, the application should run and display the screenshot shown below.
	objc_registerClassPair(ViewClass);
}
