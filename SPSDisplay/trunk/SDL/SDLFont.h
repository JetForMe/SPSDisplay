//
//  SDLFont.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__SDLFont__
#define __SPSDisplay__SDLFont__

//
//	Standard Includes
//

#include <string>
#include <vector>

//
//	Library Imports
//

#if SDLFramework
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif






class
SDLFont
{
public:
	static	void			addFontLocation(const std::string& inPath);
	static	SDLFont*		fontForName(const std::string& inName, uint8_t inPointSize);
	
							~SDLFont();
	
	int						height()		const;
	int						ascent()		const;
	int						descent()		const;
	int						lineSkip()		const;
	
	int						measureText(const std::string& inText, int16_t& outWidth, int16_t& outHeight)	const;
	
	TTF_Font*				font()	const;
	
protected:
	SDLFont(const std::string& inName, uint8_t inPointSize);
	
private:
	std::string				mFontName;
	uint8_t					mFontSize;
	TTF_Font*				mFont;
	
	static	std::vector<std::string>		sFontLocations;
};









#endif /* defined(__SPSDisplay__SDLFont__) */
