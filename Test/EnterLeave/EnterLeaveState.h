// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <FSM.h>
#include "EnterLeaveTestData.h"

class EnterLeave : public fsm::BaseState
{
public:
	EnterLeave( EnterLeaveTestData &d, std::string name, fsm::FSM &fsm );

protected:
	EnterLeaveTestData &data;

private:
	void Enter();
	void Leave();

	SetupEnterChain( EnterLeave );
	SetupLeaveChain( EnterLeave );
};

