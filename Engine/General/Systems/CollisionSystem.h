#pragma once

#include "../World.h"

struct CollisionData
{
	SmartPtr<Actor> actor1;
	SmartPtr<Actor> actor2;
	float enterTime;

	CollisionData & operator = (const CollisionData & otherCollisionData)
	{
		if (this == &otherCollisionData)
			return *this;
		actor1 = otherCollisionData.actor1;
		actor2 = otherCollisionData.actor2;
		enterTime = otherCollisionData.enterTime;
		return *this;
	}
};



class CollisionSystem
{
	World * world;
	std::vector<CollisionData> collisionDataArray;

public:
	CollisionSystem(World * _world);
	void operator()(int milliseconds);

	void Run();
};