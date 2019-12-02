#pragma once

#include "General/World.h"

class CollisionData
{
	CollisionData(Actor* actor1, Actor* actor2);
	
	Actor* actor1;
	Actor* actor2;
};

class CollisionThread
{
private:
	World* world;

public:
	CollisionThread(World* &_world);
	void operator()(int milliseconds) const;
};