//
//  SPSDisplayApp.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__App__
#define __SPSDisplay__App__

//
//	Standard Includes
//

#include <stdint.h>


class SDLDisplay;
class SDLLabel;
class SDLSurface;

class ChargeController;


class
SPSDisplayApp
{
public:
	SPSDisplayApp();
	
	virtual	bool			init();
	virtual	SDLDisplay*		createDisplay()							=	0;
	
	void					run();
	
	void					quit();

protected:
	virtual	bool			initChargeController()					=	0;
	
	static	uint32_t		timerCallbackWrapper(uint32_t inInterval, void* inParam);
	virtual	void			timerCallback();
	
			void			updateFields();
			void			updateSun();
			
protected:
	SDLDisplay*				mDisplay;
	SDLLabel*				mBatteryVoltage;
	SDLLabel*				mOutputPower;
	SDLLabel*				mLocalClock;
	SDLLabel*				mUTCClock;
	SDLLabel*				mSun;
	
	bool					mRunning;
	ChargeController*		mChargeController;
};
















#endif	//	__SPSDisplay__App__
