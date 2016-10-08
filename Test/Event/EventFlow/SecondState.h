// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "FlowBase.h"
#include "../Counter.h"

class SecondState : public FlowBase
{
public:
	SecondState( Counter& c );

	void Enter();
	void Leave();

private:
	SetupEnterChain( SecondState );
	SetupLeaveChain( SecondState );
};
