// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "FinalState.h"

FinalState::FinalState( fsm::FSM<EnterLeaveBaseState>& fsm ) : EnterLeaveBaseState( "FinalState", fsm )
{}
