// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/EventBase.h>
#include "../EventBaseState.h"
#include "../Counter.h"

class CountDownOnDestruction : public fsm::EventBase<EventBaseState>
{

public:
	CountDownOnDestruction( Counter& c ) : myCounter(c)
	{
	}

	virtual ~CountDownOnDestruction()
	{
		myCounter.Dec();
	}

	void Execute( std::shared_ptr<EventBaseState> state ) override
	{
		state->Event( *this );
	}

private:
	Counter& myCounter;
};