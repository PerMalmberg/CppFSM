// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

class Counter
{
public:
	void Inc()
	{
		++myCount;
	}

	int Get()
	{
		return myCount;
	}

private:
	int myCount = 0;
};