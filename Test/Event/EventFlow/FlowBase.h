// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <FSM/BaseState.h>
#include "../Counter.h"

class FlowBase : public fsm::BaseState<FlowBase>
{
public:
	FlowBase( Counter& c, const std::string& name );

protected:
	Counter& myCounter;
};

