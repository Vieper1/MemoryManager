#pragma once

#include "Actor.h"
#include "../Math/Math.h"
#include "Components/IActorComponent.h"
#include "World.h"
#include "Components/Physics.h"




////////////////////////////////////////////////////////////////////// Constructors and Destructors
Actor::Actor(World* world, const std::string name, Vector2 position)
{
	this->name = name;
	this->position = position;
	this->world_ = world;
}
Actor::~Actor()
{
	for (IActorComponent* component : mComponents)
	{
		delete component;
	}
}
Actor &Actor::operator = (const Actor &other)
{
	this->name = other.name;
	this->position = other.position;
	this->world_ = other.world_;
	return *this;
}
////////////////////////////////////////////////////////////////////// Constructors and Destructors











////////////////////////////////////////////////////////////////////// BeginPlay & Tick
void Actor::BeginPlay()
{
	const size_t nComponents = mComponents.size();
	for (size_t i = 0; i < nComponents; i++)
	{
		if (mComponents[i])
		{
			mComponents[i]->BeginPlay();
		}
	}
}
void Actor::Tick(const float DeltaSeconds)
{
	const size_t nComponents = mComponents.size();
	for (size_t i = 0; i < nComponents; i++)
	{
		if (mComponents[i])
		{
			mComponents[i]->Tick(DeltaSeconds);
		}
	}
}
////////////////////////////////////////////////////////////////////// BeginPlay & Tick











////////////////////////////////////////////////////////////////////// Components
void Actor::AddComponent(IActorComponent* actorComponent)
{
	if (actorComponent)
		mComponents.push_back(actorComponent);
}
////////////////////////////////////////////////////////////////////// Components














////////////////////////////////////////////////////////////////////// Components
void Actor::OnCollisionStay(SmartPtr<Actor> & otherActor)
{
	if (otherActor == world_->GetPlayerActor() || world_->GetPlayerActor() == this)
	{
		PhysicsComponent * otherComp = otherActor->GetComponent<PhysicsComponent>();
		PhysicsComponent * myComp = GetComponent<PhysicsComponent>();

		if (otherComp && myComp)
		{
			myComp->AddForce((position - otherActor->position).getNormalized() * 10.0f, true);
		}
	}
}

void Actor::OnHit(SmartPtr<Actor> & otherActor)
{
	
}
////////////////////////////////////////////////////////////////////// Components