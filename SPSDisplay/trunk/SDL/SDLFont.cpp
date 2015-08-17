//
//  SDLFont.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SDLFont.h"

//
//	Standard Includes
//

#include <cstdio>
#include <errno.h>
#include <string>

//
//	Library Imports
//

#if SDLFramework
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif

#include "SPSUtil.h"










void
SDLFont::addFontLocation(const std::string& inPath)
{
	sFontLocations.push_back(inPath);
}


SDLFont*
SDLFont::fontForName(const std::string& inName, uint8_t inPointSize)
{
	//	For now, just create a new one each time. Later, cache these…
	
	return new SDLFont(inName, inPointSize);
}
	
SDLFont::SDLFont(const std::string& inName, uint8_t inPointSize)
	:
	mFontName(inName),
	mFontSize(inPointSize),
	mFont(NULL)
{
}

SDLFont::~SDLFont()
{
	if (mFont != NULL)
	{
		::TTF_CloseFont(mFont);
	}
}


TTF_Font*
SDLFont::font()	const
{
	if (mFont == NULL)
	{
		for (auto iter = sFontLocations.begin(); iter != sFontLocations.end(); ++iter)
		{
			std::string path = *iter + mFontName;
			
			const_cast<SDLFont*> (this)->mFont = ::TTF_OpenFont(path.c_str(), mFontSize);
			if (mFont == NULL)
			{
				if (errno != ENOENT)
				{
					LZLogDebug("Unable to open font [%s]: %d %s\n", path.c_str(), errno, strerror(errno));
				}
			}
			else
			{
				return mFont;
			}
		}
		
		LZLogDebug("Unable to find font [%s]\n", mFontName.c_str());
	}
	
	return mFont;
}

int
SDLFont::height() const
{
	return ::TTF_FontHeight(font());
}

int
SDLFont::ascent() const
{
	return ::TTF_FontAscent(font());
}

int
SDLFont::descent() const
{
	return ::TTF_FontDescent(font());
}

int
SDLFont::lineSkip() const
{
	return ::TTF_FontLineSkip(font());
}

int
SDLFont::measureText(const std::string& inText, int16_t& outWidth, int16_t& outHeight) const
{
	int w;
	int h;
	int result = ::TTF_SizeUTF8(font(), inText.c_str(), &w, &h);
	outWidth = w;
	outHeight = h;
	return result;
}


std::vector<std::string>			SDLFont::sFontLocations;
