#pragma once

#include "Actor.h"
#include "../Math/Math.h"
#include "IActorComponent.h"
#include "World.h"

// Constructors and Destructors
Actor::Actor(World* world, char* name, Vector2 position)
{
	this->name = name;
	this->position = position;
	this->world_ = world;
}
Actor::~Actor()
{
	if (this->name)
		free(this->name);
	
	while (!mComponents.empty())
	{
		delete mComponents.front();
	}

	printf("Actor Destructor Called!");
}
Actor &Actor::operator = (const Actor &other)
{
	this->name = other.name;
	this->position = other.position;
	this->world_ = other.world_;
	return *this;
}
// Constructors and Destructors





// BeginPlay and Tick
void Actor::BeginPlay()
{
	const size_t nComponents = mComponents.size();
	for (size_t i = 0; i < nComponents; i++)
	{
		if (mComponents[i])
		{
			mComponents[i]->BeginPlay(*this);
		}
	}
}
void Actor::Tick()
{
	const size_t nComponents = mComponents.size();
	for (size_t i = 0; i < nComponents; i++)
	{
		if (mComponents[i])
		{
			mComponents[i]->Tick(*this);
		}
	}
}



// BeginPlay and Tick


// Components
void Actor::AddComponent(IActorComponent* actorComponent)
{
	if (actorComponent)
		mComponents.push_back(actorComponent);
}
// Components



// Collision
void Actor::OnCollisionEnter(Actor* otherActor)
{
	printf("Collided with %s", otherActor->name);
}
// Collision