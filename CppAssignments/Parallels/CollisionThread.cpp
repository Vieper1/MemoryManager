#pragma once 

#include "CollisionThread.h"
#include "General/Actor.h"
#include "General/CustomComponents.h"


CollisionThread::CollisionThread(World*& _world)
{
	this->world = _world;
}

void CollisionThread::operator()(int milliseconds) const
{
	while (!world->GetIsGameOver())
	{
		if (!world->GetIsGamePaused())
		{
			std::vector<Actor*>* actors = world->GetAllActors();
			for (size_t i = 0; i < actors->size() - 1; i++)
			{
				Actor* myActor = actors->at(i);
				const CircleColliderComponent* myCollider = myActor->GetComponent<CircleColliderComponent>();
				if (!myCollider) continue;
				
				for (size_t j = i + 1; j < actors->size(); j++)
				{
					Actor* otherActor = actors->at(j);
					const CircleColliderComponent* otherCollider = otherActor->GetComponent<CircleColliderComponent>();
					if (!otherCollider) continue;

					if (myActor->position.DistanceFrom(otherActor->position) < myCollider->GetRadius() + otherCollider->GetRadius())
					{
						myActor->OnCollisionEnter(otherActor);
					}
				}
			}
		}
	}
}
