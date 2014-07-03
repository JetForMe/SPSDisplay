//
//  TristarMPPT.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/22/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__TristarMPPT__
#define __SPSDisplay__TristarMPPT__

#include "ChargeController.h"


//
//	Standard Includes
//

#include <string>

//
//	Library Includes
//

#include <modbus/modbus.h>





class
TristarMPPT : public ChargeController
{
public:

							TristarMPPT(const std::string& inPath);
				
	virtual	void			update();
	
	virtual	float			batteryVoltage()	const					{ return mBatteryVoltage; }
	virtual	float			batteryCurrent()	const					{ return mBatteryCurrent; }
	virtual	float			arrayVoltage()		const					{ return mArrayVoltage; }
	virtual	float			arrayCurrent()		const					{ return mArrayCurrent; }
	virtual	float			inputPower()		const					{ return mOutputPower; }
	virtual	float			outputPower()		const					{ return mInputPower; }
	virtual	float			controllerTemp()	const					{ return mContollerTemp; }
	virtual	float			batteryTemp()		const					{ return mBatteryTemp; }
	virtual	float			energy()			const					{ return mEnergy; }

protected:
			void			resetValues();
			
private:
	modbus_t*		mModbus;
	bool			mConnected;
	
	float			mBatteryVoltage;
	float			mBatteryCurrent;
	float			mArrayVoltage;
	float			mArrayCurrent;
	float			mOutputPower;
	float			mInputPower;
	float			mContollerTemp;
	float			mBatteryTemp;
	float			mEnergy;
};

#endif	//	__SPSDisplay__TristarMPPT__
