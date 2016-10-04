// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "EnterChangesState.h"
#include "FinalState.h"

EnterChangesState::EnterChangesState( fsm::FSM<EnterLeaveBaseState>& fsm )
		: EnterLeaveBaseState( "EnterChangesState", fsm ), enterChain( *this )
{


}

void EnterChangesState::Enter()
{
	myFsm.SetState( std::make_unique<FinalState>( myFsm ) );
}
