//
//  TristarMPPT.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 6/22/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "TristarMPPT.h"

//
//	Standard Includes
//

#include <cstdio>
#include <errno.h>
#include <string>
#include <string.h>

//
//	Library Includes
//

#include <modbus/modbus.h>

#include "SPSUtil.h"




TristarMPPT::TristarMPPT(const std::string& inPath)
	:
	mConnected(false)
{
	resetValues();
	
	//	Open the connection to the charge controller…
	
	connect();
}

void
TristarMPPT::connect()
{
	if (mConnected)
	{
		return;
	}
	
	LZLogDebug("Opening connection to MPPT\n");
	std::string path = "/dev/cu.USA19H142P1.1";
	//mModbus = ::modbus_new_rtu(path.c_str(), 9600, 'N', 8, 1);
	//mModbus = ::modbus_new_tcp("100.100.5.237", 502);
	mModbus = ::modbus_new_tcp("100.100.5.146", 502);
	if (mModbus == NULL)
	{
		LZLogDebug("Unable to open modbus connection to %s: %s\n", path.c_str(), strerror(errno));
		return;
	}
	::modbus_set_debug(mModbus, false);
	
	int result = ::modbus_connect(mModbus);
	if (result < 0)
	{
		LZLogDebug("Unable to connect to MPPT: %s\n", strerror(errno));
		return;
	}
	
	::modbus_set_slave(mModbus, 1);
	
	mConnected = true;
}

void
TristarMPPT::resetValues()
{
	mBatteryVoltage = 0.0;
	mBatteryCurrent = 0.0;
	mArrayVoltage = 0.0;
	mArrayCurrent = 0.0;
	mInputPower = 0.0;
	mOutputPower = 0.0;
	mContollerTemp = 0.0;
	mBatteryTemp = 0.0;
	mEnergy = 0.0;
}

void
TristarMPPT::update()
{
	if (!mConnected)
	{
		return;
	}
	
	uint16_t	reg[128];
	int result = ::modbus_read_registers(mModbus, 0, 63, reg);
	if (result < 63)
	{
		LZLogDebug("Unable to read registers: %s\n", strerror(errno));
		mConnected = false;
		connect();
		return;
	}
	
	float voltageScaling = reg[0] + reg[1] / 65536.0;
	float currentScaling = reg[2] + reg[3] / 65536.0;
	
	mBatteryVoltage = reg[24] * voltageScaling / 32768.0;
	mBatteryCurrent = reg[28] * currentScaling / 32768.0;
	mArrayVoltage = reg[27] * voltageScaling / 32768.0;
	mArrayCurrent = reg[29] * currentScaling / 32768.0;
	
	mOutputPower = reg[58] * voltageScaling * currentScaling / 131072.0;
	mInputPower = reg[59] * voltageScaling * currentScaling / 131072.0;
	
	mContollerTemp = (int8_t) reg[35];
	mBatteryTemp = (int8_t) reg[36];		//	0x80 means disconnected
	
	mEnergy = reg[56];
	
#if 0
	LZLogDebug("Read %d registers\n", result);
	for (int i = 0; i < result; ++i)
	{
		LZLogDebug("Reg %02d: %5u\n", i, reg[i]);
	}
#endif

#if 1
	LZLogDebug("Voltage scaling: %f\n", voltageScaling);
	LZLogDebug("Current scaling: %f\n", currentScaling);
	LZLogDebug("Battery voltage:  %6.2f  V\n", mBatteryVoltage);
	LZLogDebug("Battery current:  %6.2f  A\n", mBatteryCurrent);
	LZLogDebug("Array voltage:    %6.2f  V\n", mArrayVoltage);
	LZLogDebug("Array current:     %6.3f A\n", mArrayCurrent);
	LZLogDebug("Output power:    %7.2f  W\n", mOutputPower);
	LZLogDebug("Input power:     %7.2f  W\n", mInputPower);
#endif
}
