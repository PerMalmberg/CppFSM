// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

#pragma once

namespace fsm {

class ILeaveChain
{
public:
	virtual void DoLeave() = 0;
};

template<typename StateClass>
class LeaveChain : ILeaveChain
{
public:

	LeaveChain( StateClass &state ) : myState( state )
	{
		myState.AddToLeaveChain( this );
	}

	void DoLeave() override
	{
		myState.Leave();
	}

private:
	StateClass &myState;
};

}