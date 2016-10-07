// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/BaseState.h>

class EventA;

class EventBaseState
		: public fsm::BaseState<EventBaseState>,
		  public fsm::EventReceiver<EventA>
{
public:
	EventBaseState( const std::string& name );

	virtual void Event( EventA& event ) = 0;

};
