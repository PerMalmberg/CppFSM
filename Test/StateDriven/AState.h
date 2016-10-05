// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include "StateDrivenBaseState.h"
#include "../Event/EventCounter.h"

class AState : public StateDrivenBaseState
{
public:
	AState( EventCounter& counter );

	void Event( std::unique_ptr<GoToBEvent> event );

	void Enter();

protected:
	SetupEnterChain( AState );
};
