#pragma once

#include "String.h"

bool String::ReadString(char*& str)
{
	int size = 0;
	char c;

	while ((c = std::cin.get()) != '\n')
	{
#pragma warning(suppress: 6308)
		str = (char*)realloc(str, sizeof(char) * ++size);
		if (str != NULL)
			str[size - 1] = c;
	}
	str[size] = '\0';
	if (str == nullptr)
		return false;
	else
		return true;
}

bool String::StringCompare(const char* str1, const char* str2)
{
	size_t i = 0;
	while (i < MAX_CHECK_LENGTH)
	{
		if (str1[i] == '\0' && str2[i] == '\0')
			return true;

		if (str1[i] == '\0' || str2[i] == '\0')
			return false;
		
		if (str1[i] != str2[i])
			return false;
		i++;
	}
	return false;
}

size_t String::GetLength(const char* str)
{
	size_t i = 0;
	while (i < MAX_CHECK_LENGTH && str[i] != '\0')
	{
		i++;
	}
	return i;
}