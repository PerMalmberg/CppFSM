// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "AState.h"
#include "BState.h"

AState::AState( EventCounter& counter )
		: StateDrivenBaseState( "A", counter ), enterChain( *this )
{
}

void AState::Event( std::unique_ptr<GoToBEvent> event )
{
	GetFSM().SetState( std::make_unique<BState>( myCounter ) );
}

void AState::Enter()
{
	GetFSM().Event( std::make_unique<GoToBEvent>() );
}