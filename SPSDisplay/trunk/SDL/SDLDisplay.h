//
//  SDLDisplay.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SDLDisplay__
#define __SPSDisplay__SDLDisplay__

#include "SDLView.h"


class SDLSurface;

class
SDLDisplay : public SDLView
{
public:
							SDLDisplay(uint16_t inWidth,
										uint16_t inHeight,
										uint8_t inDepth,
										uint32_t inFlags);
	
	void					update();
	
	SDLSurface*				surface()											{ return mSurface; }
	
protected:
	void					update(SDLView* inView);

private:
	SDLSurface*				mSurface;
};


#endif /* defined(__SPSDisplay__SDLDisplay__) */
