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
#include <string>

//
//	Library Imports
//

#if SDLFramework
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif












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
		//	On OS X, the font isn't in the working directory. On Linux, we assume it is…
		
#if __APPLE__
		std::string path = "/Users/rmann/Projects/SolarPowerStation/repo/SPSDisplay/trunk/fonts/";
#else
		std::string path = "fonts/";
#endif
		
		path += mFontName;// + ".ttf";
		
		const_cast<SDLFont*> (this)->mFont = ::TTF_OpenFont(path.c_str(), mFontSize);
		if (mFont == NULL)
		{
			std::fprintf(stderr, "Unable to open font [%s]\n", path.c_str());
		}
	}
	
	return mFont;
}
