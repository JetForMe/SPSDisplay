//
//  SDLDisplay.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SDLDisplay.h"


#include <cstdio>

//
//	Project Includes
//

#include "SDLSurface.h"






SDLDisplay::SDLDisplay(uint16_t inWidth, uint16_t inHeight, uint8_t inDepth, uint32_t inFlags)
	:
	SDLView(0, 0, inWidth, inHeight)
{
	mSurface = new SDLSurface(inWidth, inHeight, inDepth, inFlags);
}

void
SDLDisplay::update()
{
	//	Reset the offset, and draw ourself…
	
	mSurface->setOffset(0, 0);
	update(this);
	
	//	Flip the buffers…
	
	::SDL_Flip(surface()->surface());
}

void
SDLDisplay::update(SDLView* inView)
{
	//	Draw the passed-in view…
	
	SDLSurface* s = surface();
	SDL_Rect f = inView->frame();
	s->offsetBy(f.x, f.y);
	
	inView->draw(s);
	
	//	Recurse through the views, translating as necessary…
	
	int16_t		offsetX;
	int16_t		offsetY;
	s->getOffset(offsetX, offsetY);
	
	ViewVectorT& views = inView->subviews();
	for (auto iter = views.begin(); iter != views.end(); ++iter)
	{
		//	Update each view…
		
		update(*iter);
		
		//	Always set the offset back to what it was…
		
		s->setOffset(offsetX, offsetY);
	}
}
