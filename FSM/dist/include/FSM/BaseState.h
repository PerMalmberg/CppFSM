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
	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	BaseState( const std::string name ) : myName( name )
	{}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	virtual ~BaseState()
	{
		// Explicitly set this to null to show that it should not be deleted.
		myFsm = nullptr;
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void AddToEnterChain( IEnterChain* chain )
	{
		myEnterChain.push_back( chain );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void AddToLeaveChain( ILeaveChain* chain )
	{
		myLeaveChain.push_front( chain );
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void DoEnter()
	{
		for( auto state : myEnterChain )
		{
			state->DoEnter();
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	void DoLeave()
	{
		for( auto state : myLeaveChain )
		{
			state->DoLeave();
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//
	//
	///////////////////////////////////////////////////////////////////////////
	const std::string GetName() const
	{
		// Note: States are volatile so prefer to return a copy instead of
		// of a const reference because that may cause dangling references
		// at the call site.
		return myName;
	}

protected:
	const std::string myName;

	// Gets the FSM.
	// Note: Although apparent by how new states are constructed without a reference to an FSM, this method will
	// return nullptr during construction of the state, so not call it until after construction, i.e. at the earliest
	// in the Enter() chain.
	fsm::FSM<FSMBaseState>& GetFSM() { return *myFsm; }

	// Called by the FSM to keep the state alive.
	// This is intentionally placed as protected to
	// allow subclasses to call it but prevent others doing the same.
	virtual void Tick()
	{
		// Do nothing by default
	}

private:

	std::vector<IEnterChain*> myEnterChain;
	std::deque<ILeaveChain*> myLeaveChain;

	// Only ever called by the FSM when activating a new state
	void SetFSM( fsm::FSM<FSMBaseState>* fsm ) { myFsm = fsm; }
	fsm::FSM<FSMBaseState>* myFsm = nullptr;
	friend class FSM<FSMBaseState>;
};

}
