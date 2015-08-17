//
//  SPSUtil.h
//  SPSDisplay
//
//  Created by Roderick Mann on 8/17/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SPSUtil__
#define __SPSDisplay__SPSUtil__

#include <string.h>

#define LZLogDebug(format, ...)									\
	fprintf(stderr, "<%s:%d>: " format,								\
	strrchr("/" __FILE__, '/') + 1,								\
	__LINE__,													\
	## __VA_ARGS__)


#endif /* defined(__SPSDisplay__SPSUtil__) */
