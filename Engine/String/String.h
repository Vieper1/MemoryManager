#pragma once

#include <iostream>

#define MAX_CHECK_LENGTH 128

class String
{
public:
	static bool ReadString(char* &str);
	static bool StringCompare(const char* str1, const char* str2);
	static size_t GetLength(const char* str);
	static void StringCopy(const char* inStr, char* &outStr, size_t len);
};