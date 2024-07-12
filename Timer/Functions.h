#pragma once
#include <string>

namespace std
{
	unsigned int to_unsigned_number(const string& str);
}

/// <summary>
/// ��������� ������� �� ������ � �������� HH:MM:SS
/// </summary>
/// <param name="timeString">����������� ������ � �������� HH:MM:SS</param>
/// <returns></returns>
tm StringToTime(std::string& timeString);
tm NumberToTime(const unsigned long seconds);
std::string TimeToString(const tm& time);