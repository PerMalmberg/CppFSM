// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "StateThatSendsEventOnTick.h"
#include "Events/CountDownOnDestruction.h" // Important

StateThatSendsEventOnTick::StateThatSendsEventOnTick( Counter& c )
		: EventBaseState( "StateThatSendsEventOnTick" ),
		  myCounter( c )
{}

void StateThatSendsEventOnTick::Tick()
{
	// Send two events
	GetFSM().EnqueueEvent( std::make_unique<CountDownOnDestruction>(myCounter) );
	GetFSM().EnqueueEvent( std::make_unique<CountDownOnDestruction>(myCounter) );

	GetFSM().SetState( std::make_unique<StateThatSendsEventOnTick>( myCounter ) );
}

