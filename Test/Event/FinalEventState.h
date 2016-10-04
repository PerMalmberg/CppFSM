// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EventBaseState.h"

class FinalEventState : public EventBaseState
{
public:
	FinalEventState( fsm::FSM<EventBaseState>& fsm );

};


