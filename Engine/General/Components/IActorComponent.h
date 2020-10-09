#pragma once

#include "../Actor.h"
#include "../../Memory/SmartPointer/WeakPtr.h"

class IActorComponent
{
protected:
	WeakPtr<Actor> actor;
	
public:
	IActorComponent(SmartPtr<Actor> & actorRef) : actor(actorRef) {};
	virtual ~IActorComponent() = default;
	virtual void BeginPlay() {}
	virtual void Tick(const float DeltaSeconds) {}
};