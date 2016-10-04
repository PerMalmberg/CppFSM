// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EnterLeaveBaseState.h"

class LeaveChangesState : public EnterLeaveBaseState
{
public:
	LeaveChangesState( fsm::FSM<EnterLeaveBaseState>& fsm );

	SetupLeaveChain( LeaveChangesState );

private:
	void Leave();
};

