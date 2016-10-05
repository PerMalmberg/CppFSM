// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "LeaveChangesState.h"
#include "FinalState.h"

LeaveChangesState::LeaveChangesState()
		: EnterLeaveBaseState( "LeaveChangesState" ), leaveChain( *this )
{}

void LeaveChangesState::Leave()
{
	GetFSM().SetState( std::make_unique<FinalState>() );
}
