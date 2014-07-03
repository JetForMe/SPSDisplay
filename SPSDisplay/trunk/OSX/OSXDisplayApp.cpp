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


extern "C"
int
main(int inArgC, const char* inArgV[])
{
	OSXDisplayApp			app;
	app.run();

	return 0;
}
