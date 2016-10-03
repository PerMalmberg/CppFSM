// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <FSM.h>
#include <EventReceiver.h>
#include "EventBaseState.h"
#include "AddEvent.h"
#include "EventCounter.h"

class StateListeningToEvents : public EventBaseState
{
public:
	StateListeningToEvents( fsm::FSM<EventBaseState>& fsm, EventCounter& count );

	void Event( std::unique_ptr<AddEvent> event ) override;
	void Event( std::unique_ptr<SubtractEvent> event ) override;

private:
	EventCounter& myCount;
};

