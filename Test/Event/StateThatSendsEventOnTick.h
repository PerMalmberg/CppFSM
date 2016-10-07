// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EventBaseState.h"
#include "Counter.h"

class StateThatSendsEventOnTick : public EventBaseState
{
public:
	StateThatSendsEventOnTick( Counter& c );

protected:
	void Tick() override;

private:
	Counter& myCounter;
};
