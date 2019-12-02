#pragma once

#include "IActorComponent.h"
#include "World.h"

class RandomMovementComponent : public IActorComponent
{	
public:
	RandomMovementComponent(const float speed) : moveSpeed(speed) {}
	
	void Tick(Actor& actor) override
	{
		switch (Math::random(0, 5))
		{
		case 0:
			actor.position += Vector2::Right() * moveSpeed * actor.world_->GetDeltaSeconds();
			break;
		case 1:
			actor.position += Vector2::Left() * moveSpeed * actor.world_->GetDeltaSeconds();
			break;
		case 2:
			actor.position += Vector2::Up() * moveSpeed * actor.world_->GetDeltaSeconds();
			break;
		case 3:
			actor.position += Vector2::Down() * moveSpeed * actor.world_->GetDeltaSeconds();
			break;
		default:
			break;
		}
	}

private:
	float moveSpeed;
};

class WrapAroundComponent : public IActorComponent
{	
public:
	WrapAroundComponent(float xL, float yL, float xH, float yH) : xMin(xL), yMin(yL), xMax(xH), yMax(yH) {}
	
	void Tick(Actor& actor) override
	{
		const Vector2 pos = actor.position;
		if (pos.x < xMin)
			actor.position.x = xMax;
		if (pos.x > xMax)
			actor.position.x = xMin;
		if (pos.y < yMin)
			actor.position.y = yMax;
		if (pos.y > yMax)
			actor.position.y = yMin;
	}

private:
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};

class CircleColliderComponent : public IActorComponent
{
public:
	CircleColliderComponent(float r) : radius(r) {}

	void SetRadius(float r)
	{
		radius = r;
	}

	float GetRadius() const
	{
		return radius;
	}

	void Tick(Actor& actor) override
	{
		
	}

private:
	float radius = 1.f;
};