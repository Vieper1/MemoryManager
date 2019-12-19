#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "Actor.h"

// THIS IS THE RESOLUTION OF THE GRID IN WHICH YOUR GAME PLAYS
#define COLS 20
#define ROWS 20

class World
{
public:
	World();
	~World();

	std::vector<Actor*>* GetAllActors();
	Actor* SpawnActor(char* name, Vector2 spawn_location);
	void DeleteAllActors();
	void SetPlayerActor(Actor* actor);
	Actor* GetPlayerActor() const;
	void TickAll();
	void BeginPlayAll();
	void EndPlay();
	bool GetIsGameOver() const;
	bool GetIsGamePaused() const;
	void PauseToggle();
	float GetDeltaSeconds() const;

private:
	std::vector<Actor*> actors_;
	Actor* player_actor_;
	bool is_game_over_ = false;
	bool is_game_paused_ = false;
	std::chrono::steady_clock::time_point last_time_point_;
	long long delta_time_;
};