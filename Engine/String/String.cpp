#pragma once

#include "String.h"

bool String::ReadString(char* &str)
{
	int size = 0;
	char c;

	while ((c = std::cin.get()) != '\n')
	{
		if (size > 0)
		{
			char* temp = new char[size];
			StringCopy(str, temp, size);
			delete[] str;
			str = new char[++size + 1];
			str[size - 1] = c;
			StringCopy(temp, str, size - 1);
			delete[] temp;
		}
		else
		{
			str[size++] = c;
		}
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

void String::StringCopy(const char* inStr, char* &outStr, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		outStr[i] = inStr[i];
	}
}