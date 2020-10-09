#pragma once

#include <iostream>
#include <vector>

class Input
{
	static std::vector<unsigned int> * keysDown;
	
public:
	Input();
	~Input();
	static bool Keyboard_IsKeyDown(unsigned int i_VKeyID);
	static void Keyboard_KeyPress(unsigned int i_VKeyID, bool bWentDown);
};
