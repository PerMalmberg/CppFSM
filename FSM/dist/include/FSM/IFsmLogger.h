// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <string>

namespace fsm {

class IFsmLogger
{
public:
	virtual void EnteringState( const std::string state ) = 0;

	virtual void LeavingState( const std::string state ) = 0;

	virtual void ActivatingState( const std::string state ) = 0;

	virtual ~IFsmLogger()
	{}
};

}