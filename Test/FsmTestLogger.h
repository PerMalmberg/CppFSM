// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <FSM/IFsmLogger.h>

class FsmTestLogger : public fsm::IFsmLogger
{
public:
	void EnteringState( const std::string state ) override;

	void LeavingState( const std::string state ) override;

	void ActivatingState( const std::string state ) override;
};

