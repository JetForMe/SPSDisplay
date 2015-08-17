//
//  OSXDisplayApp.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__OSXDisplayApp__
#define __SPSDisplay__OSXDisplayApp__

#include "SPSDisplayApp.h"


class SDLDisplay;

/**
	Implementation of the display app on OS X to speed development.
*/

class
OSXDisplayApp : public SPSDisplayApp
{
public:
	OSXDisplayApp();
	
	virtual	SDLDisplay*		createDisplay();

protected:
	virtual	bool			initChargeController();
};


#endif /* defined(__SPSDisplay__OSXDisplayApp__) */
