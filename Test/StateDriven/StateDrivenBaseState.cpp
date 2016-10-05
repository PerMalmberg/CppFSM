// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "StateDrivenBaseState.h"

StateDrivenBaseState::StateDrivenBaseState( const std::string& name, EventCounter& counter )
		: BaseState( name ), myCounter( counter ), enterChain( *this )
{}

void StateDrivenBaseState::Enter()
{
	myCounter.count++;
}
