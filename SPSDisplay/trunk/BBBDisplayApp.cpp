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

#include "SPSUtil.h"

#include "SDLDisplay.h"
#include "SDLFont.h"

#include "PentaMetric.h"
#include "TristarMPPT.h"



BBBDisplayApp::BBBDisplayApp()
{
	SDLFont::addFontLocation("../fonts/");
}

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
	LZLogDebug("Opening TristarMPPT connection\n");
	mChargeController = new TristarMPPT("/dev/tty1");	//	TODO: what will this be?
	if (mChargeController == NULL)
	{
		return false;
	}
	
	LZLogDebug("Opening PentaMetric connection\n");
	mPentaMetric = new PentaMetric("/dev/ttyACM0");
	if (mPentaMetric == NULL)
	{
		return false;
	}
	
	return true;
}



extern "C"
int
main(int inArgC, const char* inArgV[])
{
	BBBDisplayApp			app;
	app.run();
	
	return 0;
}
