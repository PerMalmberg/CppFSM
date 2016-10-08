// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "StateListeningToEvents.h"
#include "Events/EventA.h"

StateListeningToEvents::StateListeningToEvents( Counter& c )
		: EventBaseState( "StateListeningToEvents" ),
		  myCounter( c )
{}

void StateListeningToEvents::Tick()
{
	GetFSM().EnqueueEvent( std::make_unique<EventA>() );
}

void StateListeningToEvents::Event( EventA& )
{
	myCounter.Inc();
}
