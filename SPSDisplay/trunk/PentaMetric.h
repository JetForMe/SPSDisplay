//
//  PentaMetric.h
//  SPSDisplay
//
//  Created by Roderick Mann on 8/14/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__PentaMetric__
#define __SPSDisplay__PentaMetric__

#include <string>



class
PentaMetric
{
public:
				PentaMetric(const std::string& inPath);
				
	void		resetValues();
	void		open();
	
	double		voltage() const					{ return mVoltage; }
	double		current() const					{ return mCurrent; }
	double		power() const					{ return mPower; }
	double		ampHours() const				{ return mAmpHours; }
	double		soc() const						{ return mSOC; }
	
	void		readVoltage1();
	void		readCurrent1();
	void		readPower1();
	void		readAmpHours1();
	void		readSOC1();
	
	double		readFormat1();
	double		readFormat2();
	double		readFormat2B();
	double		readFormat3();
	double		readFormat5();
	double		readFormat6();
	
	bool		verifyChecksum(const void* inBuffer, size_t inLen);
	
private:
	std::string			mPath;
	int					mFD;
	
	double				mVoltage;
	double				mCurrent;
	double				mPower;
	double				mAmpHours;
	double				mSOC;
};

#endif /* defined(__SPSDisplay__PentaMetric__) */
