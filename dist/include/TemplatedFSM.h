//
// Created by permal on 9/29/16.
//

#pragma once

#include <stdlib.h>
#include <memory>
#include "IBaseState.h"

namespace tfsm {

template<typename BaseState>
class FSM {
public:
    virtual ~FSM();
    void SetState( std::unique_ptr<BaseState> newState );

    bool HasState() { return myCurrent != nullptr; }
    BaseState& GetState() const { return *myCurrent; }
private:
    std::unique_ptr<BaseState> myCurrent = nullptr;


};

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
template<typename BaseState>
FSM<BaseState>::~FSM()
{
    myCurrent.reset();
}


///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
template<typename BaseState>
void FSM<BaseState>::SetState(std::unique_ptr<BaseState> newState)
{
    if( myCurrent ) {
        myCurrent->Leave();
    }

    myCurrent = std::move( newState );
    myCurrent->Enter();
}

}