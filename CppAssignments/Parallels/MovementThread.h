#pragma once

#include "General/World.h"

class MovementThread
{
private:
	World* world_;

public:
	MovementThread(World* &_world);
	void operator()(int milliseconds) const;
};