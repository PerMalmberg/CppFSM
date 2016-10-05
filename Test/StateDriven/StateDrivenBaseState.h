// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#ifndef FSM_ASTATE_H
#define FSM_ASTATE_H


#include <FSM/BaseState.h>
#include "../Event/EventCounter.h"
#include "Events/GoToBEvent.h"
#include "Events/GoToCEvent.h"

class StateDrivenBaseState :
		public fsm::BaseState<StateDrivenBaseState>,
		public fsm::EventReceiver<GoToBEvent>,
		public fsm::EventReceiver<GoToCEvent>
{
public:
	StateDrivenBaseState( const std::string& name, EventCounter& counter );

	void Event( std::unique_ptr<GoToBEvent> event ) override {}
	void Event( std::unique_ptr<GoToCEvent> event ) override {}

protected:
	EventCounter& myCounter;

	SetupEnterChain( StateDrivenBaseState );

private:
	void Enter();
};


#endif //FSM_ASTATE_H
