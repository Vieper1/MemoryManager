#pragma once

#include "../Actor.h"
#include "IActorComponent.h"
#include "../World.h"
#include <cstdarg>


class PhysicsComponent : public IActorComponent
{
public:
	// Fixed Arguments
	PhysicsComponent(SmartPtr<Actor> & actorRef, const float i_mass = 1.f, const float i_gravityScale = 1.f, const float i_linearDrag = 0.0001f)
		: IActorComponent(actorRef), mass(i_mass <= 0.f ? 0.0001f : i_mass), gravityScale(i_gravityScale), linearDrag(i_linearDrag)
	{}

	// Variable Arguments
	PhysicsComponent(SmartPtr<Actor> & actorRef, const size_t n ...) : IActorComponent(actorRef)
	{
		if (n < 3)
		{
			printf("NOT ENOUGH PARAMETERS FOUND! | %s @ %d\n", __FILE__, __LINE__);
		}
		else
		{
			va_list varList;
			va_start(varList, n);
			const float i_mass = static_cast<float>(va_arg(varList, double));
			mass = i_mass < 0.0001f ? 0.0001f : i_mass;
			gravityScale = static_cast<float>(va_arg(varList, double));
			linearDrag = static_cast<float>(va_arg(varList, double));
			va_end(varList);
		}
	}

	void Tick(const float DeltaSeconds) override
	{
		if (DeltaSeconds > 0.5f) return;
		SmartPtr<Actor> actor;
		this->actor.acquire(actor);
		if (!actor)
		{
			netForce = Vector2::Zero();
			netImpulse = Vector2::Zero();
		}

		AddForce(Vector2::Down() * gravityScale);

		if (linearDrag < 0.0f) linearDrag = 0.0f;
		const Vector2 currentVelocity = actor->GetVelocity();
		AddForce(currentVelocity * currentVelocity.getMagnitude() * -1 * linearDrag * DeltaSeconds);

		const float deltaSeconds = actor->world_->GetDeltaSeconds();
		const Vector2 accelFromForce = netForce * 1000.f / mass;
		const Vector2 accelFromImpulse = netImpulse / mass;
		const Vector2 newVelocity = currentVelocity + (accelFromForce * deltaSeconds) + accelFromImpulse;

		const Vector2 displacement = (currentVelocity + newVelocity) / 2.f * deltaSeconds;
		if (!xConstraint)
			actor->position.x += displacement.x;
		if (!yConstraint)
			actor->position.y += displacement.y;
		
		actor->SetVelocity(newVelocity);
		netForce = Vector2::Zero();
		netImpulse = Vector2::Zero();
	}

	void AddForce(const Vector2 input, const bool isImpulse = false)
	{
		isImpulse ? netImpulse += input : netForce += input;
	}

	void SetConstraints(const bool x, const bool y)
	{
		xConstraint = x;
		yConstraint = y;
	}

	

private:
	// Config
	float mass = 1.f;
	float gravityScale = 1.f;
	float linearDrag = 0.0001f;

	bool xConstraint = false;
	bool yConstraint = false;

	// Intermediates
	Vector2 netForce;
	Vector2 netImpulse;
};
