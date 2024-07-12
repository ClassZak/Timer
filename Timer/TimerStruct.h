#pragma once
#include <string>
#include "Functions.h"

struct TimerStruct
{
	std::string name;
	std::string description;

	tm time;

	bool paused = false;
	bool stopped = false;
	bool triggered = false;
	unsigned long long number;



	TimerStruct(std::string name,std::string description,tm time,unsigned long long number)
	{
		this->name = name;
		this->description = description;
		this->time = time;
		this->number = number;
	}
	TimerStruct(const TimerStruct& other)
		: TimerStruct(other.name, other.description,other.time,other.number)
	{
	}
};