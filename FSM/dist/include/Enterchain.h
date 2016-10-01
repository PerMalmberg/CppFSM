//
// Created by permal on 10/1/16.
//

#pragma once

namespace fsm {

class IEnterChain
{
public:
	virtual void DoEnter() = 0;
};

class ILeaveChain
{
public:
	virtual void DoLeave() = 0;
};

template<typename StateClass>
class EnterChain : IEnterChain
{
public:

	EnterChain( StateClass &state ) : myState( state )
	{
		myState.AddToEnterChain( this );
	}

	void DoEnter() override
	{
		myState.Enter();
	}

private:
	StateClass &myState;
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