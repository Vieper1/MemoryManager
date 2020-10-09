#pragma once

#include <iostream>
#include <vector>
#include "Windows.h"
#include "Actor.h"
#include "../Memory/SmartPointer/WeakPtr.h"
#include <mutex>
#include "json.hpp"

class CollisionSystem;
class Input;


class World
{
public:
	World(HINSTANCE i_hinstance, int i_cmdShow);
	~World();

	std::vector<SmartPtr<Actor>> GetAllActors() const;
	void SpawnActor(SmartPtr<Actor> & ref, const std::string name, Vector2 spawn_location);
	void SpawnPrefab(SmartPtr<Actor> & ref, const std::string filePath);
	void SpawnActorFromJson(const nlohmann::json & actorData, SmartPtr<Actor> & ref);
	void SpawnQueuedActors();
	void QueueNewActor(SmartPtr<Actor> & ref, const std::string name, Vector2 spawn_location);
	void LoadScene(const std::string filePath);
	bool RenderAllActors();
	void DeleteActor(SmartPtr<Actor> & actor);
	void DeleteAllActors();
	void SetPlayerActor(SmartPtr<Actor> & actor);
	WeakPtr<Actor> & GetPlayerActor();
	std::vector<WeakPtr<Actor>> & GetAllWeakActorRefs();
	void UpdateWeakActorRefs();
	
	void TickAll();
	void BeginPlayAll();
	void EndPlay();
	void Shutdown();
	bool GetIsGameOver() const;
	bool GetIsGamePaused() const;
	void PauseToggle();
	float GetDeltaSeconds() const;
	float GetGameTime() const;


	std::mutex CurrentActorsMutex;
	std::mutex CurrentActorRefsMutex;
	
private:
	std::vector<SmartPtr<Actor>> actors;
	std::vector<WeakPtr<Actor>> actorRefs;

	CollisionSystem * collisionSystem;
	Input * inputSystem;

	std::mutex NewActorsMutex;
	std::vector<SmartPtr<Actor>> actorsToAdd;
	

	

	WeakPtr<Actor> playerActor;
	bool isGameOver = false;
	bool isGamePaused = false;
	__int64 lastTimePoint;
	LARGE_INTEGER pcFrequency;
	float deltaSeconds;
	float totalSeconds;
};