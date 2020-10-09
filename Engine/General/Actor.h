#pragma once

#include <iostream>
#include "../Math/pchMath.h"
#include <vector>
#include "../Memory/SmartPointer/SmartPtr.h"


namespace GLib {
	namespace Sprites {
		struct Sprite;
	}
}

class IActorComponent;
class World;

enum EMobility
{
	Static,
	Moveable
};

class Actor {
public:
	Actor(World* world, const std::string name, Vector2 position);
	virtual ~Actor();
	Actor &operator = (const Actor &other);

	
	virtual void BeginPlay();
	virtual void Tick(const float DeltaSeconds);

	
	void AddComponent(IActorComponent* actorComponent);
	template <class T> T* GetComponent()
	{
		for (auto& component : mComponents)
		{
			T* ptr = dynamic_cast<T*>(component);
			if (ptr != nullptr)
				return ptr;
		}
		return nullptr;
	}

	virtual void OnHit(SmartPtr<Actor> & otherActor);
	virtual void OnCollisionStay(SmartPtr<Actor> & otherActor);




	inline Vector2 GetVelocity() const { return velocity; }
	inline void SetVelocity(const Vector2 i_velocity) { velocity = i_velocity; }

	inline EMobility GetMobility() const { return mobility; }
	inline void SetMobility(const EMobility i_mobility) { mobility = i_mobility; }



	
public:
	std::string name;
	GLib::Sprites::Sprite * sprite;
	Vector2 position = Vector2{ 0,0 };
	float zRotation;
	World * world_;
	bool bDebugTrigger = false;
	bool bDestroyedOnHit = false;
	
private:
	std::vector<IActorComponent*> mComponents;
	EMobility mobility = Moveable;
	Vector2 velocity;
};


