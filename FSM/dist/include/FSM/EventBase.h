// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

namespace fsm {

class EventBase
{
public:
	virtual void Execute() = 0;
};

}