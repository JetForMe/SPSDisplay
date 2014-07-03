//
//  SDLLabel.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SDLLabel.h"

//
//	Standard Includes
//

#include <string>

//
//	Library Imports
//

#if SDLFramework
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif

//
//	Project Includes
//

#include "SDLFont.h"
#include "SDLSurface.h"




SDLLabel::SDLLabel(const SDL_Rect& inFrame)
	:
	SDLView(inFrame)
{
	init();
}

SDLLabel::SDLLabel(int16_t inX, int16_t inY, uint16_t inWidth, uint16_t inHeight)
	:
	SDLView(inX, inY, inWidth, inHeight)
{
	init();
}

void
SDLLabel::init()
{
	mFont = NULL;
	setTextColor(0, 0, 0, 255);
	//setBackgroundColor(255, 255, 255);
	mJustification = kJustificationLeft;
	mSurface = NULL;
}

void
SDLLabel::setText(const std::string& inVal)
{
	mText = inVal;
	setNeedsDisplay();
}

void
SDLLabel::setFont(const SDLFont* inVal)
{
	mFont = inVal;
	setNeedsDisplay();
}

void
SDLLabel::setTextColor(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inAlpha)
{
	mTextColor.r = inRed;
	mTextColor.g = inGreen;
	mTextColor.b = inBlue;
	mTextColor.unused = inAlpha;
	setNeedsDisplay();
}

void
SDLLabel::setJustification(SDLJustification inJust)
{
	mJustification = inJust;
	setNeedsDisplay();
}

void
SDLLabel::updateSurface()
{
	if (needsDisplay())
	{
		delete mSurface;
		//SDL_Surface* sdlS = ::TTF_RenderUTF8_Shaded(mFont->font(), mText.c_str(), mTextColor, backgroundColor());
		SDL_Surface* sdlS = ::TTF_RenderUTF8_Blended(mFont->font(), mText.c_str(), mTextColor);
		mSurface = new SDLSurface(sdlS);
		clearNeedsDisplay();
	}
}

void
SDLLabel::draw(SDLSurface* inSurface) const
{
	const_cast<SDLLabel*> (this)->updateSurface();
	
	//	Determine where it should be placed based on the justification…
	
	SDL_Rect f = frame();
	f.y = 0.0;
#if 1
	if (mJustification == kJustificationCenter)
	{
		//	Center over the center of our frame…
		
		f.x = (frame().w - mSurface->width()) / 2;
	}
	else if (mJustification == kJustificationRight)
	{
		//	Subtract the width from our rect right edge…
		
		f.x = frame().w - mSurface->width();
	}
#endif

	inSurface->blit(mSurface, f);
}

const SDLSurface*
SDLLabel::surface() const
{
	const_cast<SDLLabel*> (this)->updateSurface();
	return mSurface;
}
