// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include "IFsmLogger.h"

namespace fsm {

class NullLogger : public IFsmLogger
{
	void EnteringState( const std::string state ) override {}

	void LeavingState( const std::string state ) override {}

	virtual void ActivatingState( const std::string state ) {}
};

}