#include "World.h"

using namespace std::chrono;

World::World()
{}


World::~World()
{}


// General Actors
std::vector<Actor*>* World::GetAllActors()
{
	return &actors_;
}
Actor* World::SpawnActor(char* name, const Vector2 spawn_location)
{
	Actor* actor = new Actor(this, name, spawn_location);
	actors_.push_back(actor);
	return actor;
}

void World::DeleteAllActors()
{
	for (size_t i = 0; i < actors_.size(); i++)
	{
		delete actors_[i];
	}
	player_actor_ = nullptr;
}


// PlayerActor
void World::SetPlayerActor(Actor* actor)
{
	player_actor_ = actor;
}
Actor* World::GetPlayerActor() const
{
	return player_actor_;
}


// Event
void World::BeginPlayAll()
{
	for (auto& actor : actors_)
	{
		actor->BeginPlay();
	}
	last_time_point_ = steady_clock::now();
}
void World::TickAll()
{
	for (auto& actor : actors_)
	{
		actor->Tick();
	}
	delta_time_ = duration_cast<microseconds>(steady_clock::now() - last_time_point_).count();
	last_time_point_ = steady_clock::now();
}
void World::EndPlay()
{
	is_game_over_ = true;
}
bool World::GetIsGameOver() const
{
	return is_game_over_;
}

bool World::GetIsGamePaused() const
{
	return is_game_paused_;
}

void World::PauseToggle()
{
	is_game_paused_ = !is_game_paused_;
}


float World::GetDeltaSeconds() const
{
	return static_cast<float>(delta_time_) / 1000000.f;
}
