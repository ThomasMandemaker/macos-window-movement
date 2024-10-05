//
//  CMacsTypes.c
//  CMacs
//
//  Created by Robert Widmann on 11/29/14.
//  Copyright (c) 2014 CodaFi. All rights reserved.
//

#include "CMacsTypes.h"

CMacsSimpleMessage cmacs_simple_msgSend = (CMacsSimpleMessage)objc_msgSend;
CMacsVoidMessage cmacs_void_msgSend	 = (CMacsVoidMessage)objc_msgSend;
CMacsVoidMessage1 cmacs_void_msgSend1 = (CMacsVoidMessage1)objc_msgSend;
CMacsRectMessage1 cmacs_rect_msgSend1 = (CMacsRectMessage1)objc_msgSend;
CMacsWindowInitMessage cmacs_window_init_msgSend = (CMacsWindowInitMessage)objc_msgSend;

// Custom
CMacsFloatMessage cmacs_float_msgSend = (CMacsFloatMessage)objc_msgSend;
CMacsColorMessage cmacs_color_msgSend = (CMacsColorMessage)objc_msgSend;
CMacsRecsMessage cmacs_rect = (CMacsRecsMessage)objc_msgSend;
CMacsVoidMessage2 cmacs_set_frame = (CMacsVoidMessage2)objc_msgSend;
CMacsSimpleMessage1 cmacs_simple_msgSend1 = (CMacsSimpleMessage1)objc_msgSend;
CMacsSimpleMessage2 cmacs_simple_msgSend2 = (CMacsSimpleMessage2)objc_msgSend;
CMacsDoubleMessage1 cmacs_double_msgSend1 = (CMacsDoubleMessage1)objc_msgSend;
CMacsPointMessage1 cmacs_point_msgSend1 = (CMacsPointMessage1)objc_msgSend;
