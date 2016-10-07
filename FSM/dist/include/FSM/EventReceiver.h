// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

namespace fsm {

template <typename EventType>
class EventReceiver
{
public:
	virtual void Event( EventType& event ) = 0;
};

}