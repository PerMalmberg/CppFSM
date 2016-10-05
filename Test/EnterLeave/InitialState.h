// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/FSM.h>
#include "EnterLeaveBaseState.h"

class InitialState : public EnterLeaveBaseState
{
public:
	InitialState() : EnterLeaveBaseState( "InitialState" )
	{

	}
};
