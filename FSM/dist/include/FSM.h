//
// Created by permal on 9/29/16.
//

#pragma once

#include <stdlib.h>
#include <memory>

namespace fsm {

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class IBaseState;

class FSM {
public:
    FSM();

    virtual ~FSM();
    void SetState( std::unique_ptr<IBaseState> newState );

    bool HasState() { return myCurrent != nullptr; }
private:
    std::unique_ptr<IBaseState> myCurrent = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
class IBaseState
{
public:
    IBaseState( std::string name, FSM& fsm ) : myFsm( fsm ), myName( name ) {}
    virtual ~IBaseState() {}

    virtual void Enter() {}
    virtual void Leave() {}

protected:
    FSM& myFsm;
    std::string myName;
private:
};


}