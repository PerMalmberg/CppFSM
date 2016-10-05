// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "StateListeningToEvents.h"
#include "FinalEventState.h"

StateListeningToEvents::StateListeningToEvents( EventCounter& count )
		: EventBaseState( "StateListeningToEvents" ), myCount( count )
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

void StateListeningToEvents::Event( std::unique_ptr<ChangeStateEvent> event )
{
	GetFSM().SetState( std::make_unique<FinalEventState>() );
}
