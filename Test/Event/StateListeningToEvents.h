// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <FSM/FSM.h>
#include <FSM/EventReceiver.h>
#include "EventBaseState.h"
#include "Events.h"
#include "EventCounter.h"

class StateListeningToEvents : public EventBaseState
{
public:
	StateListeningToEvents( EventCounter& count );

	void Event( std::unique_ptr<AddEvent> event ) override;
	void Event( std::unique_ptr<SubtractEvent> event ) override;
	void Event( std::unique_ptr<ChangeStateEvent> event ) override;

private:
	EventCounter& myCount;
};

