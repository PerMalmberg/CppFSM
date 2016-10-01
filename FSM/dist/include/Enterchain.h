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

template<typename StateClass>
class EnterChain : IEnterChain
{
public:

	EnterChain( StateClass &state ) : myState( state )
	{
		myState.AppendEnterChain( this );
	}

	void DoEnter() override
	{
		myState.Enter();
	}

private:
	StateClass &myState;
};

}