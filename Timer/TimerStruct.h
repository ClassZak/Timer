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
	TimerStruct
	(
		std::string name,
		std::string description,
		tm time,
		unsigned long long number,
		bool paused,
		bool stopped,
		bool triggered
	) : TimerStruct(name, description, time, number)
	{
		this->paused = paused;
		this->stopped = stopped;
		this->triggered = triggered;
	}
	TimerStruct(const TimerStruct& other)
		: TimerStruct(other.name, other.description,other.time,other.number,other.paused,other.stopped,other.triggered)
	{
	}

	bool operator ==(const TimerStruct& other)
	{
		return
			name == other.name and
			description == other.description and
			time.tm_sec == other.time.tm_sec and
			time.tm_min == other.time.tm_min and
			time.tm_hour == other.time.tm_hour and
			paused == other.paused and
			stopped == other.stopped and
			triggered == other.triggered and
			number == other.number;
	}
	const TimerStruct& operator =(const TimerStruct& other)
	{
		this->paused = other.paused;
		this->stopped = other.stopped;
		this->triggered = other.triggered;

		this->name = other.name;
		this->description = other.description;
		this->time = other.time;
		this->number = other.number;

		return *this;
	}
};