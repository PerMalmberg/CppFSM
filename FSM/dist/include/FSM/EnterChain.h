// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

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

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	explicit EnterChain( StateClass &state ) : myState( state )
	{
		myState.AddToEnterChain( this );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void DoEnter() override
	{
		myState.Enter();
	}

private:
	StateClass &myState;
};

}