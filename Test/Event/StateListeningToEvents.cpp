// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "StateListeningToEvents.h"

StateListeningToEvents::StateListeningToEvents( fsm::FSM<EventBaseState>& fsm, EventCounter& count )
		: EventBaseState( "StateListeningToEvents", fsm ), myCount( count )
{
}

void StateListeningToEvents::Event( std::unique_ptr<AddEvent> event )
{
	myCount.count++;
}

void StateListeningToEvents::Event( std::unique_ptr<SubtractEvent> event )
{
	myCount.count--;
}
