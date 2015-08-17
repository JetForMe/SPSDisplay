//
//  Timer.cpp
//  SPSDisplay
//
//  Created by Roderick Mann on 8/25/14.
//  Copyright (c) 2014 Latency: Zero. All rights reserved.
//

#include "Timer.h"

#include <SDL/SDL.h>


Timer::Timer()
{
}

void
Timer::schedule(uint32_t inInterval)
{
	SDL_AddTimer(inInterval, callbackWrapper, this);
}

uint32_t
Timer::callbackWrapper(uint32_t inInterval, void* inParam)
{
#if 0
	SDL_Event event = { 0 };
	event.type = SDL_USEREVENT;
	event.user.type = SDL_USEREVENT;
	SDL_PushEvent(&event);
	return inInterval;
#endif

	Timer* self = reinterpret_cast<Timer*> (inParam);
	
	try
	{
		self->exec();
	}
	
	catch (...)
	{
		fprintf(stderr, "Unexpected exception in timer callback\n");
	}
	
	return inInterval;		//	Repeat
}

void
Timer::exec()
{
}
