//
// Created by permal on 10/8/16.
//

#include "InitialFlow.h"
#include "SecondState.h"

InitialFlow::InitialFlow( Counter& c ) : FlowBase( c, "InitalFlow" ), myCounter( c )
{
}

void InitialFlow::Tick()
{
	myCounter.Inc();
	if( myCounter.Get() == 5 )
	{
		GetFSM().SetState( std::make_unique<SecondState>( myCounter ) );
	}
}
