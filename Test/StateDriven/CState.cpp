// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "CState.h"

CState::CState( EventCounter& counter )
		: StateDrivenBaseState( "C", counter )
{
}