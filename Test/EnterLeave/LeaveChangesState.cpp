// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "LeaveChangesState.h"
#include "FinalState.h"

LeaveChangesState::LeaveChangesState( fsm::FSM<EnterLeaveBaseState>& fsm )
		: EnterLeaveBaseState( "LeaveChangesState", fsm ), leaveChain( *this )
{}

void LeaveChangesState::Leave()
{
	myFsm.SetState( std::make_unique<FinalState>( myFsm ) );
}
