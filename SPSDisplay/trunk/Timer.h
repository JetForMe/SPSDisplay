//
//  Timer.h
//  SPSDisplay
//
//  Created by Roderick Mann on 8/25/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#ifndef __SPSDisplay__Timer__
#define __SPSDisplay__Timer__

#include <stdint.h>



class
Timer
{
public:
				Timer();
				
	void		schedule(uint32_t inInterval);
	
	virtual
	void		exec()		=	0;
	
	void		setContext(void* inContext)					{ mContext = inContext; }
	void*		context()									{ return mContext; }
	
private:
	static
	uint32_t
	callbackWrapper(uint32_t inInterval, void* inParam);
	
	void*		mContext;
};


#endif /* defined(__SPSDisplay__Timer__) */
