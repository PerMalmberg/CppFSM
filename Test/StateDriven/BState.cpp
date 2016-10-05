// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "BState.h"

BState::BState( EventCounter& counter )
		: StateDrivenBaseState( "B", counter )
{
}