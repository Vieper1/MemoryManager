#pragma once

#include "Actor.h"

class IActorComponent {
public:
	virtual ~IActorComponent() = default;
	virtual void BeginPlay(Actor& actor) {}
	virtual void Tick(Actor& actor) {}
};