//
//  SDLSurface.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SDLSurface.h"


//
//	Standard Includes
//

#include <cstdio>

//
//	Library Includes
//

#include <SDL/SDL.h>





SDLSurface::SDLSurface(SDL_Surface* inSurface)
	:
	mSurface(inSurface)
{
}


SDLSurface::SDLSurface(uint16_t inWidth, uint16_t inHeight, uint8_t inDepth, uint32_t inFlags)
	:
	mSurface(NULL)
{
	mSurface = ::SDL_SetVideoMode(inWidth, inHeight, inDepth, inFlags);
}


SDLSurface::~SDLSurface()
{
	if (mSurface != NULL)
	{
		::SDL_FreeSurface(mSurface);
	}
}

void
SDLSurface::blit(const SDLSurface* inSource, const SDL_Rect& inDestRect)
{
	SDL_Rect destRect = inDestRect;
	destRect.x += mOffsetX;
	destRect.y += mOffsetY;
	int result = ::SDL_BlitSurface(inSource->mSurface, NULL, mSurface, &destRect);
	if (result != 0)
	{
		std::fprintf(stderr, "Error in SDL_BlitSurface: %d\n", result);
	}
}


void
SDLSurface::fillRect(const SDL_Rect& inRect, uint8_t inRed, uint8_t inGreen, uint8_t inBlue)
{
	SDL_PixelFormat* pf = mSurface->format;
	SDL_Rect r = inRect;
	r.x += mOffsetX;
	r.y += mOffsetY;
	::SDL_FillRect(mSurface, (SDL_Rect*) &r, SDL_MapRGB(pf, inRed, inGreen, inBlue));
}
