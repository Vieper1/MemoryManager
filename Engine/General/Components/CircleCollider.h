#pragma once

#include "IActorComponent.h"
#include "../Actor.h"

class CircleColliderComponent : public IActorComponent
{
public:
	CircleColliderComponent(SmartPtr<Actor> & actorRef, float r) : IActorComponent(actorRef), radius(r) {}

	void SetRadius(float r)
	{
		radius = r;
	}

	float GetRadius() const
	{
		return radius;
	}

private:
	float radius = 1.f;
};
