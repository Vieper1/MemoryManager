#pragma once

#include "IActorComponent.h"
#include "../Actor.h"
#include "../../Math/Vector3.h"

struct AABB
{
	Vector3 Center;
	Vector3 Extents;

	AABB(const Vector3 i_center, const Vector3 i_extents) : Center(i_center), Extents(i_extents)
	{}
};

class BoxColliderComponent : public IActorComponent
{
public:
	BoxColliderComponent(SmartPtr<Actor> & actorRef, const Vector3 i_center, const Vector3 i_extents) : IActorComponent(actorRef), aabb(i_center, i_extents) {}

	void SetBounds(const Vector3 i_center, const Vector3 i_extents)
	{
		aabb.Center = i_center;
		aabb.Extents = i_extents;
	}

	AABB getBounds() const
	{
		return aabb;
	}

private:
	AABB aabb;
};
