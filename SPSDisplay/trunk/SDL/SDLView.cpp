//
//  SDLView.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SDLView.h"



//
//	Project Includes
//

#include "SDLSurface.h"






SDLView::SDLView(const SDL_Rect& inFrame)
	:
	mFrame(inFrame),
	mNeedsDisplay(false),
	mBackgroundColor({ 0 })
{
}

SDLView::SDLView(int16_t inX, int16_t inY, uint16_t inWidth, uint16_t inHeight)
	:
	mNeedsDisplay(false),
	mBackgroundColor({ 0 })
{
	SDL_Rect f = { inX, inY, inWidth, inHeight };
	mFrame = f;
}

void
SDLView::setBackgroundColor(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inAlpha)
{
	mBackgroundColor.r = inRed;
	mBackgroundColor.g = inGreen;
	mBackgroundColor.b = inBlue;
	mBackgroundColor.unused = inAlpha;
	setNeedsDisplay();
}

void
SDLView::addSubview(SDLView* inView)
{
	mSubviews.push_back(inView);
	setNeedsDisplay();
	inView->setNeedsDisplay();
}

void
SDLView::draw(SDLSurface* inSurface) const
{
	//	Draw the backround if not fully transparent…
	
	if (mBackgroundColor.unused != 0)
	{
		SDL_Rect r = frame();
		r.x = 0;
		r.y = 0;
		inSurface->fillRect(r, mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b);
	}
}

