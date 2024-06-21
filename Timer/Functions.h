#pragma once
#include <string>

namespace std
{
	unsigned int to_unsigned_number(const string& str)
	{
		unsigned int number = 0;
		for (size_t i = 0; i != str.length(); ++i)
			number += (str[i] - '0') * (unsigned int)pow(10, i);

		return number;
	}
}