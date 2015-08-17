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

//
//	Library Includes
//

#include "xively.h"




class SDLDisplay;
class SDLLabel;
class SDLSurface;

class ChargeController;
class PentaMetric;
class UpdateTimer;
class ReportTimer;



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
	
			void			updateFields();
			void			updateSun();
			void			updateDerived();
			
			void			updateXively();
			
protected:
	SDLDisplay*				mDisplay;
	SDLLabel*				mBatteryVoltage;
	SDLLabel*				mPentaMetricVoltage;
	SDLLabel*				mPentaMetricPower;
	SDLLabel*				mPentaMetricAmpHours;
	SDLLabel*				mBatteryCurrent;
	SDLLabel*				mBatteryCurrentLabel;
	SDLLabel*				mBatteryPowerLabel;
	SDLLabel*				mPowerUse;
	SDLLabel*				mSOC;
	SDLLabel*				mOutputPower;
	SDLLabel*				mLocalClock;
	SDLLabel*				mUTCClock;
	SDLLabel*				mSunrise;
	SDLLabel*				mSunset;
	SDLLabel*				mSunriseTomorrow;
	SDLLabel*				mSun;
	
	bool					mRunning;
	ChargeController*		mChargeController;
	PentaMetric*			mPentaMetric;
	
	UpdateTimer*			mUpdateTimer;
	ReportTimer*			mReportTimer;
	
	double					mSunAzimuth;
	double					mSunElevation;
	
	double					mComputedPowerUse;
	
	xi_context_t*			mXIContext;
	xi_feed_t				mXIFeed;
	
	friend class UpdateTimer;
	friend class ReportTimer;
};
















#endif	//	__SPSDisplay__App__
