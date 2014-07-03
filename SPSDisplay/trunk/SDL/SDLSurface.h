//
//  SDLSurface.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SDLSurface__
#define __SPSDisplay__SDLSurface__

//
//	Library Includes
//

#include <SDL/SDL.h>


struct SDL_Surface;


class
SDLSurface
{
public:
						SDLSurface(uint16_t inWidth, uint16_t inHeight, uint8_t inDepth, uint32_t inFlags);
						SDLSurface(SDL_Surface* inSurface);
	virtual				~SDLSurface();
	
	
	void				blit(const SDLSurface* inSource, const SDL_Rect& inDestRect);
	
	SDL_Surface*		surface()				const				{ return mSurface; }
	SDL_PixelFormat*	format()									{ return mSurface->format; }
	
	int16_t				width()					const				{ return mSurface->w; }
	int16_t				height()				const				{ return mSurface->h; }
	
	void				fillRect(const SDL_Rect& inRect, uint8_t inRed, uint8_t inGreen, uint8_t inBlue);
	
	void				offsetBy(int16_t inX, int16_t inY)			{ mOffsetX += inX; mOffsetY += inY; }
	void				setOffset(int16_t inX, int16_t inY)			{ mOffsetX = inX; mOffsetY = inY; }
	void				getOffset(int16_t& outX, int16_t& outY)		{ outX = mOffsetX; outY = mOffsetY; }
	
private:
	SDL_Surface*				mSurface;
	
	int16_t						mOffsetX;
	int16_t						mOffsetY;
};


#endif	//	__SPSDisplay__SDLSurface__
