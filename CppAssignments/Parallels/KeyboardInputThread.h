#pragma once

#include "General/World.h"

class KeyboardInputThread
{
private:
	World* world_;

public:
	KeyboardInputThread(World* &_world);
	void operator()(int milliseconds) const;
};