// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <FSM/FSM.h>
#include "EnterLeaveTestData.h"
#include "EnterLeaveBaseState.h"

class EnterLeave : public EnterLeaveBaseState
{
public:
	EnterLeave( EnterLeaveTestData &d, std::string name, fsm::FSM<EnterLeaveBaseState> &fsm );

protected:
	EnterLeaveTestData &data;

private:
	void Enter();
	void Leave();

	SetupEnterChain( EnterLeave );
	SetupLeaveChain( EnterLeave );
};

