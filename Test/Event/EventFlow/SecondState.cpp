//
// Created by permal on 10/8/16.
//

#include "SecondState.h"

SecondState::SecondState( Counter& c )
		: FlowBase( c, "SecondState" ), enterChain( *this ), leaveChain( *this )
{

}

void SecondState::Enter()
{
	myCounter.Inc();
	GetFSM().Terminate();
}

void SecondState::Leave()
{
	myCounter.Inc();
}