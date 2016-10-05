// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/FSM.h>
#include <FSM/BaseState.h>
#include "Events.h"

class EventBaseState
		: public fsm::BaseState<EventBaseState>,
		  public fsm::EventReceiver<AddEvent>,
		  public fsm::EventReceiver<SubtractEvent>,
		  public fsm::EventReceiver<ChangeStateEvent>
{
public:
	EventBaseState( const std::string& name ) :
			BaseState( name )
	{}

	virtual void Event( std::unique_ptr<AddEvent> event ) override
	{}

	virtual void Event( std::unique_ptr<SubtractEvent> event ) override
	{}

	virtual void Event( std::unique_ptr<ChangeStateEvent> event ) override
	{
	}
};