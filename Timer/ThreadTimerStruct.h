#pragma once
#include "TimerStruct.h"

struct ThreadTimerStruct
{
	void* timersTable = nullptr;
	std::size_t number = 0;
};
