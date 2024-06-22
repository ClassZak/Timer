#pragma once
#include <string>

namespace std
{
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