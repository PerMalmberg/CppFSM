// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/FSM.h>
#include <FSM/BaseState.h>

class EnterLeaveBaseState : public fsm::BaseState<EnterLeaveBaseState>
{
public:
	EnterLeaveBaseState( const std::string& name )
			: BaseState( name )
	{}

};