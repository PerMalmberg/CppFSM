//
// Created by permal on 9/29/16.
//

#pragma once

namespace tfsm {

class IBaseState
{
public:
    IBaseState( std::string name ) : myName( name ) {}
    virtual ~IBaseState() {};

    virtual void Enter() {}
    virtual void Leave() {}
    std::string GetName() { return myName; }
private:
    std::string myName;
};

}