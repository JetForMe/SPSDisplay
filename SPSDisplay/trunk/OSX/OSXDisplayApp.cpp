//
//  OSXDisplayApp.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "OSXDisplayApp.h"


//
//	Library Includes
//

#include <SDL/SDL.h>
#include <SDL_ttf/SDL_ttf.h>

//
//	Project Includes
//

#include "SDLDisplay.h"

#include "TristarMPPT.h"






SDLDisplay*
OSXDisplayApp::createDisplay()
{
	SDLDisplay* display = new SDLDisplay(480, 272, 0, SDL_SWSURFACE | SDL_DOUBLEBUF);
	return display;
}

bool
OSXDisplayApp::initChargeController()
{
	mChargeController = new TristarMPPT("/dev/cu.USA19H142P1.1");
	return mChargeController != NULL;
}


void
test()
{
	if (::SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		fprintf(stderr, "SDL_Init() failed. %s\n", SDL_GetError());
		return;
	}
	
	if (::TTF_Init() != 0)
	{
		fprintf(stderr, "TTF_Init() failed. %s\n", TTF_GetError());
		return;
	}
	
	SDL_Surface* mSurface = ::SDL_SetVideoMode(480, 272, 0, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_Rect r = { 0, 0, 480, 272 };
	::SDL_FillRect(mSurface, &r, SDL_MapRGB(mSurface->format, 255, 255, 255));
	
	TTF_Font* font = TTF_OpenFont("/Users/rmann/Projects/SolarPowerStation/repo/SPSDisplay/trunk/fonts/HelveticaNeue-Regular.ttf", 12);
	SDL_Color textColor = { 0 };
	SDL_Surface* sdlS = ::TTF_RenderUTF8_Blended(font, "Sun: 2014-07-02", textColor);
	SDL_Rect destRect = { 0 };
	destRect.x = 30;
	destRect.y = 30;
	int result = ::SDL_BlitSurface(sdlS, NULL, mSurface, &destRect);
	if (result != 0)
	{
		std::fprintf(stderr, "SDL_BlitSurface returned error: %d\n", result);
		return;
	}
	
	bool mRunning = true;
	while (mRunning)
	{
		SDL_Event event;
		if (SDL_WaitEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					mRunning = false;
					break;
				}
			}
			
			::SDL_Flip(mSurface);
		}
		
		SDL_Delay(1);
	}
}

extern "C"
int
main(int inArgC, const char* inArgV[])
{
	OSXDisplayApp			app;
	
#if 0
	test();
#else
	app.run();
#endif

	return 0;
}
