// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EnterLeaveBaseState.h"

class FinalState : public EnterLeaveBaseState
{
public:
	FinalState( fsm::FSM<EnterLeaveBaseState>& fsm );

};

