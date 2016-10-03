// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM.h>

class InitialState : public fsm::BaseState
{
public:
	InitialState( fsm::FSM &fsm ) : BaseState( "InitialState", fsm )
	{

	}
};
