//
// Created by permal on 10/4/16.
//

#pragma once

#include "FSM.h"

namespace fsm {

template <typename FSMBaseState>
class BaseState
{
public:
	BaseState( const std::string name, FSM<FSMBaseState>& fsm ) : myFsm( fsm ), myName( name )
	{}

	virtual ~BaseState()
	{}

	void AddToEnterChain( IEnterChain *chain )
	{
		myEnterChain.push_back( chain );
	}

	void AddToLeaveChain( ILeaveChain *chain )
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

protected:
	FSM<FSMBaseState>& myFsm;
	const std::string myName;
private:
	std::vector<IEnterChain*> myEnterChain;
	std::deque<ILeaveChain*> myLeaveChain;
};

}
