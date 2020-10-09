#pragma once

#include "../Actor.h"
#include "IActorComponent.h"
#include "../World.h"
#include "Physics.h"


class FollowComponent : public IActorComponent
{
public:
	// Fixed Arguments
	FollowComponent(SmartPtr<Actor> & actorRef, WeakPtr<Actor> i_targetActorRef, const bool i_bShouldFollow = true, const float i_followForce = 1.0f)
		: IActorComponent(actorRef), targetActorRef(i_targetActorRef), bShouldFollow(i_bShouldFollow), followForce(i_followForce)
	{}

	// Variable Arguments
	FollowComponent(SmartPtr<Actor> & actorRef) : IActorComponent(actorRef)
	{}

	void BeginPlay() override
	{
		SmartPtr<Actor> actor;
		this->actor.acquire(actor);

		physicsComp = actor->GetComponent<PhysicsComponent>();
	}

	void Tick(const float DeltaSeconds) override
	{
		SmartPtr<Actor> actor;
		this->actor.acquire(actor);

		SmartPtr<Actor> targetActor;
		this->targetActorRef.acquire(targetActor);

		
		if (!physicsComp)
			physicsComp = actor->GetComponent<PhysicsComponent>();
		

		if (targetActor && physicsComp && bShouldFollow)
		{
			const Vector2 direction = (targetActor->position - actor->position).getNormalized();
			physicsComp->AddForce(direction * followForce);
		}
	}

	void SetFollow(WeakPtr<Actor> & i_targetActorRef, const bool i_bShouldFollow)
	{
		targetActorRef = i_targetActorRef;
		bShouldFollow = i_bShouldFollow;
	}

public:
	WeakPtr<Actor> targetActorRef;
	bool bShouldFollow = false;
	float followForce = 1.0f;

private:
	PhysicsComponent * physicsComp = nullptr;
};
