//
//  BBBDisplayApp.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "BBBDisplayApp.h"






//	Library Includes
//

#include <SDL/SDL.h>

//
//	Project Includes
//

#include "SDLDisplay.h"

#include "TristarMPPT.h"




SDLDisplay*
BBBDisplayApp::createDisplay()
{
	SDLDisplay* display = new SDLDisplay(480, 272, 0, SDL_FULLSCREEN);
	SDL_ShowCursor(0);			//	Hide the cursor
	return display;
}


bool
BBBDisplayApp::initChargeController()
{
	mChargeController = new TristarMPPT("/dev/tty1");	//	TODO: what will this be?
	return mChargeController != NULL;
}



extern "C"
int
main(int inArgC, const char* inArgV[])
{
	BBBDisplayApp			app;
	app.run();
	
	return 0;
}
