// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

namespace fsm {

template <typename FSMBaseState>
class EventBase
{
public:
	virtual ~EventBase() {}

	virtual void Execute( std::shared_ptr<FSMBaseState> state ) = 0;
};

}