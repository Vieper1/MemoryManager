#include "World.h"
#include "Windows.h"
#include "../Render/GLibHelper.h"
#include "GLib/GLib.h"
#include <fstream>
#include "Components/BoxCollider.h"
#include "Systems/Input.h"
#include "Components/Physics.h"
#include "Components/Follow.h"
#include "Systems/CollisionSystem.h"
using json = nlohmann::json;


//////////////////////////////////////////////////////////////////////////////////////////////////// INIT/EXIT
World::World(HINSTANCE i_hinstance, int i_cmdShow)
{
	collisionSystem = new CollisionSystem(this);
	inputSystem = new Input();

	const bool bSuccess = GLib::Initialize(i_hinstance, i_cmdShow, "Viper Engine Output", -1, 800, 600);
	assert(bSuccess);

	GLib::SetKeyStateChangeCallback(Input::Keyboard_KeyPress);
}


World::~World()
{
	delete collisionSystem;
	delete inputSystem;
}
//////////////////////////////////////////////////////////////////////////////////////////////////// INIT/EXIT











//////////////////////////////////////////////////////////////////////////////////////////////////// ACTORS
std::vector<SmartPtr<Actor>> World::GetAllActors() const
{
	return actors;
}
void World::SpawnActor(SmartPtr<Actor> & ref, const std::string name, const Vector2 spawn_location)
{
	CurrentActorsMutex.lock();
	const SmartPtr<Actor> actor(new Actor(this, name, spawn_location));
	ref = actor;
	actors.push_back(actor);
	UpdateWeakActorRefs();
	CurrentActorsMutex.unlock();
}

void World::SpawnQueuedActors()
{
	if (!actorsToAdd.empty())
	{
		CurrentActorsMutex.lock();
		for (SmartPtr<Actor> queuedActor : actorsToAdd)
		{
			actors.push_back(queuedActor);
		}
		actorsToAdd.clear();
		CurrentActorsMutex.unlock();
	}
}


void World::QueueNewActor(SmartPtr<Actor>& ref, const std::string name, Vector2 spawn_location)
{
	NewActorsMutex.lock();
	const SmartPtr<Actor> actor(new Actor(this, name, spawn_location));
	ref = actor;
	actorsToAdd.push_back(actor);
	NewActorsMutex.unlock();
}






void World::LoadScene(const std::string filePath)
{
	std::ifstream file(filePath, std::ios_base::in);
	std::string fileData(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
	auto editorState = json::parse(fileData);

	for (const json & actorData : editorState["actors"])
	{
		SmartPtr<Actor> ref;
		SpawnActorFromJson(actorData, ref);
	}
}

void World::SpawnPrefab(SmartPtr<Actor> & ref, const std::string filePath)
{
	std::ifstream file(filePath, std::ios_base::in);
	std::string fileData(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
	auto actorData = json::parse(fileData);
	SpawnActorFromJson(actorData, ref);
}

void World::SpawnActorFromJson(const nlohmann::json & actorData, SmartPtr<Actor> & ref)
{
	// Set Position
	Vector2 spawnPos = Vector2::Zero();
	float spawnRot = 0.0f;
	Vector2 spawnVelocity = Vector2::Zero();
	if (actorData.contains("initial-position"))
	{
		const json & posData = actorData["initial-position"];
		spawnPos = { posData[0], posData[1] };
	}

	if (actorData.contains("initial-rotation"))
	{
		const json & posData = actorData["initial-rotation"];
		spawnRot = posData.get<float>();
	}

	if (actorData.contains("initial-velocity"))
	{
		const json & velData = actorData["initial-velocity"];
		spawnVelocity = { velData[0], velData[1] };
	}

	// Set Name
	std::string actorName;
	if (actorData.contains("name"))
	{
		const json jName = actorData["name"];
		actorName = jName.get<std::string>();
	}

	////////// Spawn Actor
	SpawnActor(ref, actorName, spawnPos);
	ref->zRotation = spawnRot * 3.14159265359f / 180.f;
	ref->SetVelocity(spawnVelocity);

	if (actorData.contains("mobility") && actorData["mobility"].get<std::string>() == "static")
		ref->SetMobility(EMobility::Static);

	// Set Sprite
	if (actorData.contains("sprite-path"))
	{
		auto spritePath = actorData["sprite-path"].get<std::string>();
		ref->sprite = GLibHelper::CreateSprite(spritePath.data());
	}

	// Set Components
	for (const json & comp : actorData["components"])
	{
		auto componentType = comp["type"].get<std::string>();
		const size_t nParams = comp.size();

		// Physics
		if (componentType == "PhysicsComponent")
		{
			const float mass = comp["mass"].get<float>();
			const float gravityScale = comp["gravity-scale"].get<float>();
			const float linearDrag = comp["linear-drag"].get<float>();
			PhysicsComponent * newPhysicsComp = new PhysicsComponent(
				ref,
				nParams,
				mass,
				gravityScale,
				linearDrag
			);
			ref->AddComponent(newPhysicsComp);

			if (comp.contains("constraints"))
			{
				const json & constraints = comp["constraints"];
				newPhysicsComp->SetConstraints(constraints[0].get<bool>(), constraints[1].get<bool>());
			}
		}

		// BoxCollider
		if (componentType == "BoxColliderComponent")
		{
			const Vector3 boxExtents{ comp["size"][0], comp["size"][1], 0.f };
			ref->AddComponent(new BoxColliderComponent(ref, Vector3::Zero(), boxExtents));
		}

		// Follow
		if (componentType == "FollowComponent")
		{
			auto followComp = new FollowComponent(ref);
			if (playerActor)
			{
				followComp->targetActorRef = playerActor;
				followComp->bShouldFollow = comp["should-follow"].get<bool>();
				followComp->followForce = comp["follow-force"].get<float>();
			}
			ref->AddComponent(followComp);
		}
	}

	// Set Player Actor
	if (actorData.contains("player-actor") && actorData["player-actor"].get<bool>())
		playerActor = ref;
}

void World::DeleteActor(SmartPtr<Actor> & actor)
{
	for (size_t i = 0; i < actors.size(); i++)
	{
		SmartPtr<Actor> & cmpActor = actors.at(i);
		if (cmpActor == actor)
		{
			//actors[i] = nullptr;
			const auto spriteRef = cmpActor->sprite;
			if (spriteRef)
			{
				GLib::Sprites::Release(spriteRef);
				cmpActor->sprite = nullptr;
			}
			actors.erase(actors.begin() + i);
			break;
		}
	}
	UpdateWeakActorRefs();
}

void World::DeleteAllActors()
{
	playerActor = nullptr;
	for (SmartPtr<Actor> & ref : actors)
	{
		if (ref->sprite)
			GLib::Sprites::Release(ref->sprite);
		ref = nullptr;
	}
	actors.clear();
	UpdateWeakActorRefs();
}


// PlayerActor
void World::SetPlayerActor(SmartPtr<Actor> & actor)
{
	playerActor = actor;
}
WeakPtr<Actor> & World::GetPlayerActor()
{
	return playerActor;
}


// WeakRefs
void World::UpdateWeakActorRefs()
{
	CurrentActorRefsMutex.lock();
	actorRefs.clear();
	for (SmartPtr<Actor> & actor : GetAllActors())
		actorRefs.emplace_back(actor);
	CurrentActorRefsMutex.unlock();
}
std::vector<WeakPtr<Actor>>& World::GetAllWeakActorRefs()
{
	return actorRefs;
}
//////////////////////////////////////////////////////////////////////////////////////////////////// ACTORS












//////////////////////////////////////////////////////////////////////////////////////////////////// RENDER
bool World::RenderAllActors()
{
	// Pre-Render
	bool serviceInput;
	GLib::Service(serviceInput);
	if (serviceInput) return false;

	GLib::BeginRendering();
	GLib::Sprites::BeginRendering();

	for (WeakPtr<Actor> & weakRef : actorRefs)
	{
		SmartPtr<Actor> actor;
		weakRef.acquire(actor);
		if (actor && actor->sprite)
		{
			GLib::Point2D Offset = { actor->position.x, actor->position.y };
			RenderSprite(*actor->sprite, Offset, actor->zRotation);
		}
	}

	// Post-Render
	GLib::Sprites::EndRendering();
	GLib::EndRendering();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////// RENDER












//////////////////////////////////////////////////////////////////////////////////////////////////// EVENTS
void World::BeginPlayAll()
{
	CurrentActorsMutex.lock();
	for (WeakPtr<Actor> & weakRef : actorRefs)
	{
		SmartPtr<Actor> actor;
		weakRef.acquire(actor);
		if (actor) actor->BeginPlay();
	}

	// Time keeping
	LARGE_INTEGER lgInt;
	QueryPerformanceCounter(&lgInt);
	lastTimePoint = lgInt.QuadPart;
	CurrentActorsMutex.unlock();
}
void World::TickAll()
{
	CurrentActorsMutex.lock();
	for (WeakPtr<Actor> & weakRef : actorRefs)
	{
		SmartPtr<Actor> actor;
		weakRef.acquire(actor);
		if (actor) actor->Tick(deltaSeconds);
	}

	collisionSystem->Run();

	// Time keeping
	LARGE_INTEGER lgInt;
	QueryPerformanceCounter(&lgInt);
	QueryPerformanceFrequency(&pcFrequency);
	deltaSeconds = float(lgInt.QuadPart - lastTimePoint) / float(pcFrequency.QuadPart);
	lastTimePoint = lgInt.QuadPart;
	totalSeconds += deltaSeconds;
	CurrentActorsMutex.unlock();
}
void World::EndPlay()
{
	isGameOver = true;
}
bool World::GetIsGameOver() const
{
	return isGameOver;
}
bool World::GetIsGamePaused() const
{
	return isGamePaused;
}
void World::PauseToggle()
{
	isGamePaused = !isGamePaused;
}


float World::GetDeltaSeconds() const
{
	return static_cast<float>(deltaSeconds);
}
float World::GetGameTime() const
{
	return totalSeconds;
}
void World::Shutdown()
{
	DeleteAllActors();
	GLib::Shutdown();
	delete this;
}
//////////////////////////////////////////////////////////////////////////////////////////////////// EVENTS