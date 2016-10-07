// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EventBaseState.h"
#include "Counter.h"

class StateListeningToEvents : public EventBaseState
{
public:
	StateListeningToEvents(Counter &c);

	void Event( EventA& event ) override ;

protected:
	void Tick() override;

private:
	Counter &myCounter;
};
