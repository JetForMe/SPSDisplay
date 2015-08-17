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

#include "SPSUtil.h"

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
	mDecimalOffset = 0;
	mOffsetWidth = 0;
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
SDLLabel::setDecimalOffset(int16_t inOffset)
{
	mDecimalOffset = inOffset;
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
		
		//	Compute the offset width. If using decimal justification,
		//	we need to split the string on the decimal point. If not,
		//	just use the whole string…
		
		std::string text;
		if (mJustification == kJustificationDecimal)
		{
			size_t pos = mText.rfind(".");
			if (pos == std::string::npos)
			{
				text = mText;
			}
			else
			{
				//	We found the last decimal point, split the string here…
				
				text = mText.substr(pos);
			}
		}
		else
		{
			text = mText;
		}
		
		int16_t ignoreHeight;
		int result = mFont->measureText(text, mOffsetWidth, ignoreHeight);
		if (result != 0)
		{
			LZLogDebug("meaureText returned %d\n", result);
		}
		
		clearNeedsDisplay();
	}
}

void
SDLLabel::draw(SDLSurface* inSurface) const
{
	const_cast<SDLLabel*> (this)->updateSurface();
	
	//	Determine where it should be placed based on the justification…
	
	SDL_Rect f = frame();
	f.x = 0;
	f.y = 0.0;
	if (mJustification == kJustificationCenter)
	{
		//	Center over the center of our frame…
		
		f.x = (frame().w - mOffsetWidth) / 2;
	}
	else if (mJustification == kJustificationRight)
	{
		//	Subtract the width from our rect right edge…
		
		f.x = frame().w - mOffsetWidth;
	}
	else if (mJustification == kJustificationDecimal)
	{
		if (mDecimalOffset >= 0)
		{
			f.x = frame().w - mDecimalOffset - mOffsetWidth;
		}
		else
		{
			f.x = -mDecimalOffset - mOffsetWidth;
		}
	}

	inSurface->blit(mSurface, f);
}

const SDLSurface*
SDLLabel::surface() const
{
	const_cast<SDLLabel*> (this)->updateSurface();
	return mSurface;
}
