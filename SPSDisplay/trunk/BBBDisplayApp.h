//
//  BBBDisplayApp.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/21/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__File__
#define __SPSDisplay__File__

#include "SPSDisplayApp.h"


class SDLDisplay;

/**
	Implementation of the display app on OS X to speed development.
*/

class
BBBDisplayApp : public SPSDisplayApp
{
public:
	BBBDisplayApp();
	
	virtual	SDLDisplay*		createDisplay();

protected:
	virtual	bool			initChargeController();
};

#endif /* defined(__SPSDisplay__File__) */
