//
//  SPSDisplayApp.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/20/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "SPSDisplayApp.h"

//
//	Standard Includes
//

#include <time.h>

//
//	Library Includes
//

#include <SDL/SDL.h>
#if SDLFramework
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif

#include "spa.h"

//
//	Project Includes
//

#include "SDLDisplay.h"
#include "SDLFont.h"
#include "SDLLabel.h"
#include "SDLSurface.h"

#include "TristarMPPT.h"









SPSDisplayApp::SPSDisplayApp()
	:
	mDisplay(NULL),
	mBatteryVoltage(NULL),
	mOutputPower(NULL),
	mLocalClock(NULL),
	mUTCClock(NULL),
	mSun(NULL),
	mRunning(false),
	mChargeController(NULL)
{
}

bool
SPSDisplayApp::init()
{
	if (::SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		fprintf(stderr, "SDL_Init() failed. %s\n", SDL_GetError());
		return false;
	}
	
	if (::TTF_Init() != 0)
	{
		fprintf(stderr, "TTF_Init() failed. %s\n", TTF_GetError());
		return false;
	}
	
	mDisplay = createDisplay();
	
	initChargeController();
	
	//	Create the fonts we’ll use…
	
	//SDLFont* labelFont = SDLFont::fontForName("HelveticaNeue-Regular", 12);
	SDLFont* labelFont = SDLFont::fontForName("HelveticaNeueLTStd-Roman.otf", 12);
	//SDLFont* labelFontBold = SDLFont::fontForName("HelveticaNeue-Bold.ttf", 12);
	SDLFont* mainValueFont = SDLFont::fontForName("HelveticaNeue-Bold.ttf", 48);
	
	//	Build views…
	
	mDisplay->setBackgroundColor(255, 255, 255);
	
	SDLView* v = new SDLView(10, 10, 460, 252);
	//v->setBackgroundColor(0, 255, 0);					//	Debugging
	mDisplay->addSubview(v);
	
	//	Battery Voltage…
	
	SDLLabel* l = new SDLLabel(0, 0, 0, 0);
	l->setFont(labelFont);
	l->setText("Battery Voltage");
	v->addSubview(l);
	
	mBatteryVoltage = new SDLLabel(0, 5, 121, 0);
	mBatteryVoltage->setFont(mainValueFont);
	mBatteryVoltage->setJustification(kJustificationRight);
	mBatteryVoltage->setText("-.--");
	
	//	Output Power…
	
	l = new SDLLabel(0, 60, 0, 0);
	l->setFont(labelFont);
	l->setText("Output Power");
	v->addSubview(l);
	
	mOutputPower = new SDLLabel(0, 65, 80, 0);
	mOutputPower->setFont(mainValueFont);
	mOutputPower->setJustification(kJustificationRight);
	mOutputPower->setText("----.-");
	
	v->addSubview(mBatteryVoltage);
	v->addSubview(mOutputPower);

	//	Current time…
	
	{
		mLocalClock = new SDLLabel(200, 0, 220, 20);
		mLocalClock->setFont(labelFont);
		mLocalClock->setText("---- -- -- --:--:-- ---");
		mLocalClock->setJustification(kJustificationRight);
		v->addSubview(mLocalClock);
		
		l = new SDLLabel(425, 0, 35, 20);
		l->setFont(labelFont);
		l->setText("PDT");
		v->addSubview(l);
		
		mUTCClock = new SDLLabel(200, 20, 220, 20);
		mUTCClock->setFont(labelFont);
		mUTCClock->setText("---- -- -- --:--:-- ---");
		mUTCClock->setJustification(kJustificationRight);
		v->addSubview(mUTCClock);
		
		l = new SDLLabel(425, 20, 35, 20);
		l->setFont(labelFont);
		l->setText("UTC");
		v->addSubview(l);
		
		mSun = new SDLLabel(200, 40, 220, 20);
		mSun->setFont(labelFont);
		mSun->setText("Sun: ---°, --°");
		mSun->setJustification(kJustificationRight);
		v->addSubview(mSun);
	}
	
	//	One-time update…
	
	mDisplay->update();

	return true;
}

void
SPSDisplayApp::run()
{
	init();
	
	//	Update the charge controller variables once…
	
	mChargeController->update();
	
	//	Start a 1-second timer to update the controller…
	
	SDL_AddTimer(1000, timerCallbackWrapper, this);
	
	mRunning = true;
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
				
				case SDL_KEYDOWN:
				{
					printf("Key down: %d\n", event.key.keysym.sym);
					break;
				}
				
				case SDL_USEREVENT:
				{
					updateFields();
					break;
				}
				
				default:
				{
					//printf("Event: %d\n", event.type);
				}
			}
			
			mDisplay->update();
		}
		
		SDL_Delay(1);
	}
	
	quit();
}

void
SPSDisplayApp::updateFields()
{
	//	Only user events should be the timer, so update the charge controller…
	
	mChargeController->update();
	
	//	Update the fields…
	
	char s[16];
	std::snprintf(s, 64, "%4.1f", mChargeController->batteryVoltage());
	mBatteryVoltage->setText(s);
	
	std::snprintf(s, 64, "%4.0f", mChargeController->outputPower());
	mOutputPower->setText(s);
	
	//	Update the clock…
	
	time_t time = ::time(NULL);
	tm* localTime = ::localtime(&time);
	if (localTime != NULL)
	{
		char st[32];
		size_t r = ::strftime(st, sizeof (st), "%Y-%m-%d %H:%M:%S", localTime);
		if (r > 0)
		{
			mLocalClock->setText(st);
		}
	}
	tm* utcTime = ::gmtime(&time);
	if (utcTime != NULL)
	{
		char st[32];
		size_t r = ::strftime(st, sizeof (st), "%Y-%m-%d %H:%M:%S", utcTime);
		if (r > 0)
		{
			mUTCClock->setText(st);
		}
	}
	
	updateSun();
}

void
SPSDisplayApp::updateSun()
{
	time_t time = ::time(NULL);
	tm* localTime = ::localtime(&time);
	
	spa_data sd;
	sd.year = localTime->tm_year;
	sd.month = localTime->tm_mon;
	sd.day = localTime->tm_mday;
	sd.hour = localTime->tm_hour;
	sd.minute = localTime->tm_min;
	sd.second = localTime->tm_sec;
	sd.delta_ut1 = -0.3;		//	As of 8 May 2014 (http://maia.usno.navy.mil/ser7/ser7.dat)
	sd.timezone = -7.0;
	
	sd.longitude = -119.2;		//	Black Rock
	sd.latitude = 40.8;
	sd.elevation = 1190.0;
	
	sd.longitude = -122.064055;	//	Matterport parking lot
	sd.latitude = 37.387034;
	sd.elevation = 30.8;
	
	sd.pressure = 2062.0;		//	average pressure from here: http://www.wrcc.dri.edu/htmlfiles/westcomp.bp.html
	sd.temperature = 27.0;
	sd.slope = 0.0;
	sd.azm_rotation = 0.0;
	sd.atmos_refract = 0.5667;
	sd.function = SPA_ZA_RTS;
	
	int result = spa_calculate(&sd);
	if (result != 0)
	{
		std::fprintf(stderr, "spa_calculate returned: %d\n", result);
	}
	else
	{
		char s[32];
		std::snprintf(s, sizeof (s), "Sun: %6.3f°, %5.3f°", sd.azimuth, 90 - sd.zenith);
		mSun->setText(s);
	}
}

void
SPSDisplayApp::quit()
{
	::SDL_Quit();
}




uint32_t
SPSDisplayApp::timerCallbackWrapper(uint32_t inInterval, void* inParam)
{
	SDL_Event event = { 0 };
	event.type = SDL_USEREVENT;
	event.user.type = SDL_USEREVENT;
	SDL_PushEvent(&event);
	return inInterval;
	
	SPSDisplayApp* self = reinterpret_cast<SPSDisplayApp*> (inParam);
	
	try
	{
		self->timerCallback();
	}
	
	catch (...)
	{
		fprintf(stderr, "Unexpected exception in timer callback\n");
	}
	
	return inInterval;		//	Repeat
}

void
SPSDisplayApp::timerCallback()
{
	
}

