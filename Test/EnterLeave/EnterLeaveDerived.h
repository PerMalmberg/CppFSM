// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "EnterLeaveState.h"

class EnterLeaveDerived : public EnterLeave
{
public:
	EnterLeaveDerived( EnterLeaveTestData &d );

private:
	void Enter();
	void Leave();

	SetupEnterChain( EnterLeaveDerived );
	SetupLeaveChain( EnterLeaveDerived );
};

