//
// Created by permal on 9/29/16.
//

#pragma once

#include <stdlib.h>
#include <memory>
#include <vector>
#include "Enterchain.h"

namespace fsm {

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

	void AppendEnterChain( IEnterChain *chain );

protected:
	FSM &myFsm;
	std::string myName;
private:
	std::vector<IEnterChain *> myEnterChain;

};


}