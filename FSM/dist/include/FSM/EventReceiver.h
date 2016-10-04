// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <memory>
#include "State.h"

namespace fsm {

template<typename EventType>
class EventReceiver
{
public:
	virtual void Event( std::unique_ptr<EventType> event ) = 0;
};

}