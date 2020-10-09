#include "Input.h"

std::vector<unsigned int> * Input::keysDown;

Input::Input()
{
	keysDown = new std::vector<unsigned int>();
}

Input::~Input()
{
	delete keysDown;
}


void Input::Keyboard_KeyPress(unsigned int i_VKeyID, bool bWentDown)
{
	if (bWentDown)
		keysDown->push_back(i_VKeyID);
	else
	{
		for (unsigned int i = 0; i < keysDown->size(); i++)
		{
			if ((*keysDown)[i] == i_VKeyID)
			{
				keysDown->erase(keysDown->begin() + i);
			}
		}
	}
}

bool Input::Keyboard_IsKeyDown(unsigned i_VKeyID)
{
	for (unsigned int key : *keysDown)
	{
		if (key == i_VKeyID)
			return true;
	}
	return false;
}