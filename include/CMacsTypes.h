//
//  CMacsTypes.h
//  CMacs
//
//  Created by Robert Widmann on 11/29/14.
//  Copyright (c) 2014 CodaFi. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <objc/message.h>
#include <objc/runtime.h>

#ifndef CMACS_CMACSTYPES_H
#define CMACS_CMACSTYPES_H

typedef struct AppDel {
	Class isa;
	
	// Will be an NSWindow later.
	id window;
} AppDelegate;

enum {
	NSBorderlessWindowMask		= 0,
	NSTitledWindowMask			= 1 << 0,
	NSClosableWindowMask		= 1 << 1,
	NSMiniaturizableWindowMask	= 1 << 2,
	NSResizableWindowMask		= 1 << 3,
};

typedef id(*CMacsSimpleMessage)(id, SEL);
typedef id(*CMacsFloatMessage)(id, SEL, double);
typedef id(*CMacsColorMessage)(id, SEL, double, double, double, double);
typedef void(*CMacsVoidMessage)(id, SEL);
typedef void(*CMacsVoidMessage1)(id, SEL, void *);
typedef id(*CMacsRectMessage1)(id, SEL, CGRect);
typedef id(*CMacsWindowInitMessage)(id, SEL, CGRect, int, int, bool);

typedef CGRect(*CMacsRecsMessage)(id,SEL);
typedef void(*CMacsVoidMessage2)(id, SEL, void *, void *);
typedef id(*CMacsSimpleMessage1)(id, SEL, void *);
typedef id(*CMacsSimpleMessage2)(id, SEL, void *, void *);
typedef double(*CMacsDoubleMessage1)(id, SEL);
typedef CGPoint(*CMacsPointMessage1)(id, SEL);


extern CMacsSimpleMessage cmacs_simple_msgSend;
extern CMacsVoidMessage cmacs_void_msgSend;
extern CMacsVoidMessage1 cmacs_void_msgSend1;
extern CMacsRectMessage1 cmacs_rect_msgSend1;
extern CMacsWindowInitMessage cmacs_window_init_msgSend;

// Customer
extern CMacsFloatMessage cmacs_float_msgSend;
extern CMacsColorMessage cmacs_color_msgSend;
extern CMacsRecsMessage cmacs_rect;
extern CMacsVoidMessage2 cmacs_set_frame;
extern CMacsSimpleMessage1 cmacs_simple_msgSend1;
extern CMacsSimpleMessage2 cmacs_simple_msgSend2;
extern CMacsDoubleMessage1 cmacs_double_msgSend1;
extern CMacsPointMessage1 cmacs_point_msgSend1;
#endif
