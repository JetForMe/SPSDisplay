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

#include <cmath>
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

#include "xively.h"
#include "xi_helpers.h"

#include "spa.h"

//
//	Project Includes
//

#include "SPSUtil.h"

#include "SDLDisplay.h"
#include "SDLFont.h"
#include "SDLLabel.h"
#include "SDLSurface.h"

#include "PentaMetric.h"
#include "Timer.h"
#include "TristarMPPT.h"




class
UpdateTimer : public Timer
{
public:
	virtual void			exec();
};

class
ReportTimer : public Timer
{
public:
	virtual void			exec();
};




void
hourToHourMinSec(double inHour, uint8_t& outHour, uint8_t& outMin, uint8_t& outSec)
{
	outHour = std::trunc(inHour);
	
	inHour -= outHour;
	inHour *= 60.0;
	outMin = std::trunc(inHour);
	
	inHour -= outMin;
	inHour *= 60.0;
	outSec = std::trunc(inHour);
}

#pragma mark -



SPSDisplayApp::SPSDisplayApp()
	:
	mDisplay(NULL),
	mBatteryVoltage(NULL),
	mPentaMetricVoltage(NULL),
	mPentaMetricPower(NULL),
	mPentaMetricAmpHours(NULL),
	mBatteryCurrent(NULL),
	mSOC(NULL),
	mOutputPower(NULL),
	mLocalClock(NULL),
	mUTCClock(NULL),
	mSunrise(NULL),
	mSunset(NULL),
	mSunriseTomorrow(NULL),
	mSun(NULL),
	mRunning(false),
	mChargeController(NULL),
	mPentaMetric(NULL),
	mUpdateTimer(NULL),
	mReportTimer(NULL)
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
	
	mUpdateTimer = new UpdateTimer();
	mUpdateTimer->setContext(this);
	mReportTimer = new ReportTimer();
	mReportTimer->setContext(this);
	
	mDisplay = createDisplay();
	
	initChargeController();
	
	mPentaMetric->open();
	mPentaMetric->readVoltage1();
	mPentaMetric->readCurrent1();
	
	//	Create the fonts we’ll use…
	
	//SDLFont* labelFont = SDLFont::fontForName("HelveticaNeue-Regular", 12);
	//SDLFont* labelFont = SDLFont::fontForName("Univers LT Std 53 Extended.otf", 12);
	//SDLFont* labelFont = SDLFont::fontForName("SourceSansPro-Regular.otf", 12);
	//SDLFont* labelFontBold = SDLFont::fontForName("HelveticaNeue-Bold.ttf", 12);
	//SDLFont* mainValueFont = SDLFont::fontForName("Univers LT Std 63 Bold Extended.otf", 48);
	//SDLFont* mainValueFont = SDLFont::fontForName("SourceSansPro-Bold.otf", 48);
	
	SDLFont* labelFont = SDLFont::fontForName("Verdana.ttf", 12);
	SDLFont* mainValueFont = SDLFont::fontForName("Verdana Bold.ttf", 48);
	
	//	Build views…
	
	mDisplay->setBackgroundColor(255, 255, 255);
	
	SDLView* v = new SDLView(10, 10, 460, 252);
	//v->setBackgroundColor(0, 255, 0);					//	Debugging
	mDisplay->addSubview(v);
	
	//	Battery Voltage…
	
	SDLLabel* l = new SDLLabel(3, -5, 0, 0);
	l->setFont(labelFont);
	l->setText("Charge Voltage");
	v->addSubview(l);
	
	mBatteryVoltage = new SDLLabel(0, 2, 120, 0);
	mBatteryVoltage->setFont(mainValueFont);
	//mBatteryVoltage->setJustification(kJustificationDecimal);
	//mBatteryVoltage->setDecimalOffset(20);
	mBatteryVoltage->setText("––––.–");
	v->addSubview(mBatteryVoltage);
	
	//	PV Charge Power…
	
	l = new SDLLabel(3, 60, 0, 0);
	l->setFont(labelFont);
	l->setText("PV Charge Power");
	v->addSubview(l);
	
	mOutputPower = new SDLLabel(0, 67, 120, 0);
	mOutputPower->setFont(mainValueFont);
	//mOutputPower->setJustification(kJustificationDecimal);
	//mOutputPower->setDecimalOffset(20);
	mOutputPower->setText("––––");
	v->addSubview(mOutputPower);
	
	//	PentaMetric Voltage…
	
	int16_t yl = 130;
	int16_t yd = yl + 7;
	l = new SDLLabel(3, yl, 0, 0);
	l->setFont(labelFont);
	l->setText("PentaMetric Voltage");
	v->addSubview(l);
	
	mPentaMetricVoltage = new SDLLabel(0, yd, 70, 0);
	mPentaMetricVoltage->setFont(mainValueFont);
	//mPentaMetricVoltage->setJustification(kJustificationDecimal);
	//mPentaMetricVoltage->setDecimalOffset(20);
	mPentaMetricVoltage->setText("–.–");
	v->addSubview(mPentaMetricVoltage);
	
	//	BatteryCurrent…
	
	mBatteryCurrentLabel = new SDLLabel(145, yl, 0, 0);
	mBatteryCurrentLabel->setFont(labelFont);
	mBatteryCurrentLabel->setText("Battery Current");
	v->addSubview(mBatteryCurrentLabel);
	
	mBatteryCurrent = new SDLLabel(142, yd, 120, 0);
	mBatteryCurrent->setFont(mainValueFont);
	//mBatteryCurrent->setJustification(kJustificationDecimal);
	//mBatteryCurrent->setDecimalOffset(20);
	mBatteryCurrent->setText("–.–");
	v->addSubview(mBatteryCurrent);
	
	//	PentaMetric Power…
	
	mBatteryPowerLabel = new SDLLabel(318, yl, 0, 0);
	mBatteryPowerLabel->setFont(labelFont);
	mBatteryPowerLabel->setText("Power");
	v->addSubview(mBatteryPowerLabel);
	
	mPentaMetricPower = new SDLLabel(315, yd, 115, 0);
	mPentaMetricPower->setFont(mainValueFont);
	mPentaMetricPower->setJustification(kJustificationLeft);
	mPentaMetricPower->setDecimalOffset(20);
	mPentaMetricPower->setText("–.–");
	v->addSubview(mPentaMetricPower);
	
	//	PentaMetric AmpHours…
	
	yl = 193;
	yd = yl + 7;
	l = new SDLLabel(3, yl, 0, 0);
	l->setFont(labelFont);
	l->setText("Amp•Hours");
	v->addSubview(l);
	
	mPentaMetricAmpHours = new SDLLabel(0, yd, 70, 0);
	mPentaMetricAmpHours->setFont(mainValueFont);
	//mPentaMetricAmpHours->setJustification(kJustificationDecimal);
	//mPentaMetricAmpHours->setDecimalOffset(20);
	mPentaMetricAmpHours->setText("–.–");
	v->addSubview(mPentaMetricAmpHours);
	
	//	Power Use…
	
	l = new SDLLabel(145, yl, 100, 0);
	l->setFont(labelFont);
	l->setJustification(kJustificationLeft);
	l->setText("Power Use");
	v->addSubview(l);
	
	mPowerUse = new SDLLabel(142, yd, 120, 0);
	mPowerUse->setFont(mainValueFont);
	mPowerUse->setText("–---");
	v->addSubview(mPowerUse);
	
	//	SoC…
	
	l = new SDLLabel(318, yl, 100, 0);
	l->setFont(labelFont);
	l->setJustification(kJustificationLeft);
	l->setText("SoC");
	v->addSubview(l);
	
	mSOC = new SDLLabel(315, yd, 100, 0);
	mSOC->setFont(mainValueFont);
	mSOC->setJustification(kJustificationLeft);
	mSOC->setText("–--%");
	v->addSubview(mSOC);
	
	//	Current time…
	
	{
		mLocalClock = new SDLLabel(200, 0, 220, 20);
		mLocalClock->setFont(labelFont);
		mLocalClock->setText("–– –– ––:––:–– –––");
		mLocalClock->setJustification(kJustificationRight);
		v->addSubview(mLocalClock);
		
		l = new SDLLabel(425, 0, 35, 20);
		l->setFont(labelFont);
		l->setText("PDT");
		v->addSubview(l);
		
		mUTCClock = new SDLLabel(200, 20, 220, 20);
		mUTCClock->setFont(labelFont);
		mUTCClock->setText("–– –– ––:––:–– –––");
		mUTCClock->setJustification(kJustificationRight);
		v->addSubview(mUTCClock);
		
		l = new SDLLabel(425, 20, 35, 20);
		l->setFont(labelFont);
		l->setText("UTC");
		v->addSubview(l);
	}
	
	//	Sun related…
	
	{
		l = new SDLLabel(284, 40, 35, 20);
		l->setFont(labelFont);
		l->setText("Sunrise:");
		v->addSubview(l);
		
		l = new SDLLabel(284, 60, 35, 20);
		l->setFont(labelFont);
		l->setText("Sunset:");
		v->addSubview(l);
		
		l = new SDLLabel(284, 80, 35, 20);
		l->setFont(labelFont);
		l->setText("Sunrise:");
		v->addSubview(l);
		
		mSunrise = new SDLLabel(320, 40, 100, 20);
		mSunrise->setFont(labelFont);
		mSunrise->setJustification(kJustificationRight);
		mSunrise->setText("––:––");
		v->addSubview(mSunrise);
		
		mSunset = new SDLLabel(320, 60, 100, 20);
		mSunset->setFont(labelFont);
		mSunset->setJustification(kJustificationRight);
		mSunset->setText("––:––");
		v->addSubview(mSunset);
		
		mSunriseTomorrow = new SDLLabel(320, 80, 100, 20);
		mSunriseTomorrow->setFont(labelFont);
		mSunriseTomorrow->setJustification(kJustificationRight);
		mSunriseTomorrow->setText("––:––");
		v->addSubview(mSunriseTomorrow);
		
		mSun = new SDLLabel(284, 100, 250, 20);
		mSun->setFont(labelFont);
		mSun->setText("Sun: –––.–––°, ––.–––°");
		//mSun->setJustification(kJustificationRight);
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
	mPentaMetric->readVoltage1();
	mPentaMetric->readCurrent1();
	mPentaMetric->readPower1();
	mPentaMetric->readAmpHours1();
	mPentaMetric->readSOC1();
	
	
	//	Start timers…
	
	mUpdateTimer->schedule(1000);
	mReportTimer->exec();
	mReportTimer->schedule(5 * 60 * 1000);
	
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
				
#if 0
				case SDL_USEREVENT:
				{
					updateFields();
					break;
				}
#endif

				default:
				{
					//printf("Event: %d\n", event.type);
				}
			}
		}
		
		SDL_Delay(1);
	}
	
	quit();
}

void
SPSDisplayApp::updateFields()
{
	//	Update the charge controller…
	
	mChargeController->update();
	
	//	Update the PentaMetric…
	
	mPentaMetric->readVoltage1();
	mPentaMetric->readCurrent1();
	mPentaMetric->readPower1();
	mPentaMetric->readAmpHours1();
	mPentaMetric->readSOC1();
	
	//	Update derived values…
	
	updateDerived();
	
	//	Update the display…
	
	//	TriStar…
	
	char s[16];
	std::snprintf(s, 64, "%-4.1f", mChargeController->batteryVoltage());
	mBatteryVoltage->setText(s);
	
	std::snprintf(s, 64, "%-4.0f", mChargeController->outputPower());
	mOutputPower->setText(s);
	
	//	PentaMetric…
	
	std::snprintf(s, 64, "%-4.1f", mPentaMetric->voltage());
	mPentaMetricVoltage->setText(s);
	
	double v = mPentaMetric->power();
	if (v == 0.0)
	{
		std::snprintf(s, 64, "Power");
	}
	else if (v < 0.0)
	{
		std::snprintf(s, 64, "Power (In)");
	}
	else
	{
		std::snprintf(s, 64, "Power (Out)");
	}
	mBatteryPowerLabel->setText(s);
	
	v = abs(v);
	std::snprintf(s, 64, "%-4.0f", v);
	mPentaMetricPower->setText(s);
	
	std::snprintf(s, 64, "%-6.1f", mPentaMetric->ampHours());
	mPentaMetricAmpHours->setText(s);
	
	v = mPentaMetric->current();
	if (v == 0.0)
	{
		std::snprintf(s, 64, "Battery Current");
	}
	else if (v < 0.0)
	{
		std::snprintf(s, 64, "Battery Current (In)");
	}
	else
	{
		std::snprintf(s, 64, "Battery Current (Out)");
	}
	mBatteryCurrentLabel->setText(s);
	
	v = abs(v);
	std::snprintf(s, 64, "%-5.1f", v);
	mBatteryCurrent->setText(s);
	
	//	Power Use…
	
	std::snprintf(s, 64, "%-4.0f", mComputedPowerUse);
	mPowerUse->setText(s);
	
	//	SoC…
	
	std::snprintf(s, 64, "%-3.0f%%", mPentaMetric->soc());
	mSOC->setText(s);
	
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
	
	//sd.longitude = -122.064055;	//	Matterport parking lot
	//sd.latitude = 37.387034;
	//sd.elevation = 0;//30.8;
	
	sd.pressure = 2062.0;		//	average pressure from here: http://www.wrcc.dri.edu/htmlfiles/westcomp.bp.html
	sd.temperature = 27.0;
	sd.slope = 0.0;
	sd.azm_rotation = 0.0;
	sd.atmos_refract = 0.5667;
	sd.function = SPA_ZA_RTS;
	
	spa_data sdTomorrow = sd;
	sdTomorrow.day += 1;
	spa_calculate(&sdTomorrow);
	int result = spa_calculate(&sd);
	if (result != 0)
	{
		LZLogDebug("spa_calculate returned: %d\n", result);
	}
	else
	{
		char str[32];
		
		uint8_t h, m, s;
		hourToHourMinSec(sd.sunrise, h, m, s);
		std::snprintf(str, sizeof (str), "%02u:%02u:%02u", h, m, s);
		mSunrise->setText(str);
		
		hourToHourMinSec(sd.sunset, h, m, s);
		std::snprintf(str, sizeof (str), "%02u:%02u:%02u", h, m, s);
		mSunset->setText(str);
		
		hourToHourMinSec(sdTomorrow.sunrise, h, m, s);
		std::snprintf(str, sizeof (str), "%02u:%02u:%02u", h, m, s);
		mSunriseTomorrow->setText(str);
		
		mSunAzimuth = sd.azimuth;
		mSunElevation = 90.0 - sd.zenith;
		
		std::snprintf(str, sizeof (str), "Sun: %6.3f°, %5.3f°", mSunAzimuth, mSunElevation);
		mSun->setText(str);
	}
}

void
SPSDisplayApp::updateDerived()
{
	mComputedPowerUse = mChargeController->outputPower() + mPentaMetric->power();
	std::fprintf(stderr, "cc pwr %f, pm pwr %f, use %f\n",
					mChargeController->outputPower(),
					mPentaMetric->power(),
					mComputedPowerUse);
}

void
SPSDisplayApp::updateXively()
{
	
	//	Set up Xively…
	
	mXIContext = ::xi_create_context(XI_HTTP, "ZHbhttFm23Yau6Iu5kEhXUfg3F2jiHsadaal4Lw22OHEV5xi", 2021806591);
	if (mXIContext == NULL)
	{
		LZLogDebug("Unable to create Xively context\n");
	}
	else
	{
		::memset(&mXIFeed, 0, sizeof (mXIFeed));
		mXIFeed.feed_id = 2021806591;
		mXIFeed.datastream_count = 9;
	}
	
	//	Send the sun info…
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[0];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "sun-elevation", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mSunElevation);
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[1];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "sun-azimuth", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mSunAzimuth);
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[2];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "cc-output-power", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mChargeController->outputPower());
	}
	
	//	Battery Info…
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[3];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "bat-voltage", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mPentaMetric->voltage());
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[4];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "bat-power", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mPentaMetric->power());
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[5];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "bat-soc", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mPentaMetric->soc());
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[8];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "bat-temperature", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mChargeController->batteryTemp());
	}
	
	//	PV Info…
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[6];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "pv-current", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mChargeController->arrayCurrent());
	}
	
	{
		xi_datastream_t& ds = mXIFeed.datastreams[7];
		::xi_str_copy_untiln(ds.datastream_id, sizeof (ds.datastream_id), "pv-voltage", '\0');
		xi_datapoint_t& dp = ds.datapoints[0];
		ds.datapoint_count = 1;
		::xi_set_value_f32(&dp, mChargeController->arrayVoltage());
	}
	
	const xi_response_t* ctx = ::xi_feed_update(mXIContext, &mXIFeed);
	if (ctx != NULL)
	{
		if (ctx->http.http_status != 200)
		{
			std::fprintf(stderr, "Error writing to Xively: %d\n", ctx->http.http_status);
		}
	}
	else
	{
		std::fprintf(stderr, "WTF, response is NULL\n");
	}
	
	::xi_delete_context(mXIContext);
}


void
SPSDisplayApp::quit()
{
	::TTF_Quit();
	::SDL_Quit();
}


#pragma mark -

void
UpdateTimer::exec()
{
	SPSDisplayApp* app = reinterpret_cast<SPSDisplayApp*>(context());
	app->updateFields();
	app->updateSun();
	
	app->mDisplay->update();
}

void
ReportTimer::exec()
{
	SPSDisplayApp* app = reinterpret_cast<SPSDisplayApp*>(context());
	app->updateXively();
}
