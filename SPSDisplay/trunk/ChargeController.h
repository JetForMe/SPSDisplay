//
//  ChargeController.h
//  SPSDisplay
//
//  Created by Roderick Mann on 6/22/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__ChargeController__
#define __SPSDisplay__ChargeController__


/**
	This class represents a connected charge controller. A subclass implements the
	TriStar MPPT.
*/

class
ChargeController
{
public:
	virtual	void			update()									=	0;
	
	virtual	float			batteryVoltage()	const					=	0;
	virtual	float			batteryCurrent()	const					=	0;
	virtual	float			arrayVoltage()		const					=	0;
	virtual	float			arrayCurrent()		const					=	0;
	virtual	float			inputPower()		const					=	0;
	virtual	float			outputPower()		const					=	0;
	virtual	float			controllerTemp()	const					=	0;
	virtual	float			batteryTemp()		const					=	0;
	virtual	float			energy()			const					=	0;
	
};


#endif	//	__SPSDisplay__ChargeController__
