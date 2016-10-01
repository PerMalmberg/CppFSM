// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file. 
// Give credit where credit is due.

#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>
#include <deque>
#include "Enterchain.h"

namespace fsm {

#define SetupEnterChain( clazz ) \
	const fsm::EnterChain<clazz> enterChain; \
	friend class fsm::EnterChain<clazz>

#define SetupLeaveChain( clazz ) \
	const fsm::LeaveChain<clazz> leaveChain; \
	friend class fsm::LeaveChain<clazz>

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class BaseState;

class FSM
{
public:
	FSM();

	virtual ~FSM();

	void SetState( std::unique_ptr<BaseState> newState );

	bool HasState()
	{ return myCurrent != nullptr; }

private:
	std::unique_ptr<BaseState> myCurrent = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class BaseState
{
public:
	BaseState( std::string name, FSM &fsm ) : myFsm( fsm ), myName( name )
	{}

	virtual ~BaseState()
	{}

	void DoEnter();

	void DoLeave();
	void AddToEnterChain( IEnterChain *chain );
	void AddToLeaveChain( ILeaveChain *chain );

protected:
	FSM &myFsm;
	std::string myName;
private:
	std::vector<IEnterChain*> myEnterChain;
	std::deque<ILeaveChain*> myLeaveChain;

};


}