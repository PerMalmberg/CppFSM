// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "FSM.h"

namespace fsm {

template<typename FSMBaseState>
class BaseState
{
public:
	BaseState( const std::string name, FSM <FSMBaseState>& fsm ) : myFsm( fsm ), myName( name )
	{}

	virtual ~BaseState()
	{}

	void AddToEnterChain( IEnterChain* chain )
	{
		myEnterChain.push_back( chain );
	}

	void AddToLeaveChain( ILeaveChain* chain )
	{
		myLeaveChain.push_front( chain );
	}

	void DoEnter()
	{
		for( auto state : myEnterChain )
		{
			state->DoEnter();
		}
	}

	void DoLeave()
	{
		for( auto state : myLeaveChain )
		{
			state->DoLeave();
		}
	}

	const std::string GetName() const
	{
		// Note: States are volatile so prefer to return a copy instead of
		// of a const reference because that may cause dangling references
		// at the callee.
		return myName;
	}

protected:
	FSM <FSMBaseState>& myFsm;
	const std::string myName;
private:
	std::vector<IEnterChain*> myEnterChain;
	std::deque<ILeaveChain*> myLeaveChain;
};

}
