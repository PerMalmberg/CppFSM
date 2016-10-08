// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include "FsmTestLogger.h"

void FsmTestLogger::EnteringState( const std::string state )
{
	std::cout << "Entering: " << state << "\r\n";
}

void FsmTestLogger::LeavingState( const std::string state )
{
	std::cout << "Leaving: " << state << "\r\n";
}

void FsmTestLogger::ActivatingState( const std::string state )
{
	std::cout << "Activating: " << state << "\r\n";
}
