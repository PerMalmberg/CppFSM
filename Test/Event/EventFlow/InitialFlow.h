// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include "FlowBase.h"
#include "../Counter.h"

class InitialFlow : public FlowBase
{
public:
	InitialFlow( Counter& c );

protected:
	void Tick() override;

private:
	Counter& myCounter;
};

