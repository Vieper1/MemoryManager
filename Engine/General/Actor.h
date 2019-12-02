#pragma once

#include <iostream>
#include "../Math/Math.h"
#include <vector>


class IActorComponent;
class World;

class Actor {
public:
	Actor(World* world, char* name, Vector2 position);
	virtual ~Actor();
	Actor &operator = (const Actor &other);

	
	void BeginPlay();
	void Tick();

	
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

	void OnCollisionEnter(Actor* otherActor);

	
	Vector2 position = Vector2{ 0,0 };
	char* name;
	World* world_;

private:
	std::vector<IActorComponent*> mComponents;
};


