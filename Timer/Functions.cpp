#pragma once
#include "Functions.h"

namespace std
{
	/// <summary>
	/// В функцию надо обязательно подавать верные входные данные
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	unsigned int to_unsigned_number(const string& str)
	{
		unsigned int number = 0;
		for (size_t i = 0; i != str.length(); ++i)
		{
			if (i != 0)
				number *= 10;
			number += str[i] - '0';
		}

		return number;
	}
}

tm StringToTime(std::string& timeString)
{
	std::string hoursString = timeString.substr(0, 2);
	std::string minutesString = timeString.substr(3, 2);
	std::string secondsString = timeString.substr(6, 2);

	if (hoursString[0] == '0')
		hoursString = hoursString.substr(1, 1);
	if (minutesString[0] == '0')
		minutesString = minutesString.substr(1, 1);
	if (secondsString[0] == '0')
		secondsString = secondsString.substr(1, 1);

	tm result
	{
		(int)std::to_unsigned_number(secondsString),
		(int)std::to_unsigned_number(minutesString),
		(int)std::to_unsigned_number(hoursString)
	};

	return result;
}

tm NumberToTime(const unsigned long seconds)
{
	tm time{};
	time.tm_sec = seconds % 60;
	time.tm_min = (seconds / 60) % 60;
	time.tm_hour = (seconds / 60 / 60) % 24;

	return time;
}

std::string TimeToString(const tm& time)
{
	return
		(time.tm_hour < 10 ? std::string("0") + std::to_string(time.tm_hour) : std::to_string(time.tm_hour)) + ":" +
		(time.tm_min < 10 ? std::string("0") + std::to_string(time.tm_min) : std::to_string(time.tm_min)) + ":" +
		(time.tm_sec < 10 ? std::string("0") + std::to_string(time.tm_sec) : std::to_string(time.tm_sec));
}
