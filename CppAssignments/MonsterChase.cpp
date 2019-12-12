// CppAssignments.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Engine.h"
#include <iostream>
#include <Windows.h>
#include <cassert>
#include "Memory/MemoryManager.h"
#include "General/CustomComponents.h"
#include "General/World.h"
#include "String/String.h"
#include <thread>
#include "Parallels/KeyboardInputThread.h"
#include "Parallels/CollisionThread.h"

using namespace HeapManagerProxy;
extern bool HeapManager_UnitTest();

MemoryManager* manager;








////////////////////////////////////////////////////////////////////// Declarations
#define ROWS 10
#define COLS 10
char grid[ROWS][COLS];
////////////////////////////////////////////////////////////////////// Declarations






// Custom new() and delete()
// new() with manager
void* operator new(size_t n)
{
	void* ptr = _alloc(manager, n);
	return ptr;
}

// new() with manager and alignment
void* operator new(size_t n, unsigned int alignment)
{
	void* ptr = _alloc(manager, n, alignment);
	return ptr;
}

// new[] with manager
void* operator new[](size_t n)
{
	void* ptr = _alloc(manager, n);
	return ptr;
}

// new[] with manager and alignment
void* operator new[](size_t n, unsigned int alignment)
{
	void* ptr = _alloc(manager, n, alignment);
	return ptr;
}

// delete
void operator delete(void * p) noexcept
{
	_free(manager, p);
}













////////////////////////////////////////////////////////////////////// Utilities
void copyActorArray(Actor** &fromActors, Actor** &toActors, int index) {
	for (int i = 0; i < index; i++) {
		toActors[i] = fromActors[i];
	}
}



////////////////////////////// GRID
void initGrid() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			grid[i][j] = '-';
		}
	}
}
void printGrid(World* world) {
	initGrid();
	std::cout << "\n";
	if (world->GetAllActors()->empty()) return;
	
	for (auto& actor : *world->GetAllActors())
	{
		grid[static_cast<int>(actor->position.y)][static_cast<int>(actor->position.x)] = (actor == world->GetPlayerActor()) ? 'P' : 'M';
	}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			std::cout << grid[ROWS - i - 1][j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}
void printStates(World* world) {
	for (auto& actor : *world->GetAllActors())
	{
		if (actor == world->GetPlayerActor())
			printf("\n(%.2f, %.2f) => (Player: ", actor->position.x + 1, actor->position.y + 1);
		else
			printf("\n(%.2f, %.2f) => (Monster: ", actor->position.x + 1, actor->position.y + 1);
		
		if (actor->name)
			printf("%s)", actor->name);
	}
	printf("\n\n");
}
Vector2 randomPosition() {
	return Vector2{
		(float)Math::random(0, COLS - 1),
		(float)Math::random(0, ROWS - 1)
	};
}
////////////////////////////// GRID



// Allocation
void addMonster(Actor** &monsters, int &nMonsters) {
	if (nMonsters != 0) {
		Actor** temp;
		temp = new Actor*[nMonsters];
		copyActorArray(monsters, temp, nMonsters);
		delete[] monsters;
		monsters = new Actor*[(size_t)nMonsters + 1];
		copyActorArray(temp, monsters, nMonsters);
		delete[] temp;
	}

	//monsters[nMonsters] = new Actor();
	std::cout << "\n\nEnter the name of the monster \n->";
	if (!String::ReadString(monsters[nMonsters]->name)) return;
	monsters[nMonsters]->position = randomPosition();
	nMonsters++;
}
void removeMonster(Actor** &monsters, int &nMonsters, int location) {
	if (nMonsters == 0) return;
	delete monsters[location];
	for (int i = location; i < nMonsters - 1; i++) {
		monsters[i] = monsters[i + 1];
		monsters[i + 1] = NULL;
	}
	Actor** temp = new Actor*[--nMonsters];
	copyActorArray(monsters, temp, nMonsters);
	delete[] monsters;
	monsters = new Actor*[nMonsters];
	copyActorArray(temp, monsters, nMonsters);
	delete[] temp;
}




////////////////////////////// GAME STATE
bool checkGameOver(Actor* &player, Actor** &monsters, int &nMonsters) {
	for (int i = 0; i < nMonsters; i++) {
		if (monsters[i]->position.x == player->position.x && monsters[i]->position.y == player->position.y) {
			return true;
		}
		for (int j = i + 1; j < nMonsters; j++) {
			if (monsters[i]->position.x == monsters[j]->position.x && monsters[i]->position.y == monsters[j]->position.y) {
				removeMonster(monsters, nMonsters, i);
				std::cout << "\n-> A monster has died!\n";
			}
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////// Utilities


















#define CUSTOM_TEST
int main()
{
#ifndef CUSTOM_TEST
	HeapManager_UnitTest();
	return 0;
#else
	{
		// INIT CUSTOM MEMORY MANGER
		const size_t 		sizeHeap = 1024 * 1024;
		const unsigned int 	numDescriptors = 2048;
		void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, sizeHeap);
		manager = create(pHeapMemory, sizeHeap, numDescriptors);
		

		// START THE GAME
		int nMoves = 0;
		char* name = new char[1];

		

		// World Init
		World* world = new World();

		

		// Player Init
		std::cout << "Enter player name\n->";
		String::ReadString(name);
		Actor* player = world->SpawnActor(name, Vector2(static_cast<int>(ROWS / 2), static_cast<int>(COLS / 2)));
		player->AddComponent(new WrapAroundComponent(0, 0, COLS - 1, ROWS - 1));
		player->AddComponent(new CircleColliderComponent(1.f));
		world->SetPlayerActor(player);


		
		// Create TestMonster
		std::cout << "\nEnter monster name\n->";
		name = new char[1];
		String::ReadString(name);
		Actor *monster = world->SpawnActor(name, Vector2(0, 0));
		monster->AddComponent(new RandomMovementComponent(10.f));
		monster->AddComponent(new WrapAroundComponent(0, 0, COLS, ROWS));
		monster->AddComponent(new CircleColliderComponent(1.f));


		
		// Begin Simulation
		world->BeginPlayAll();
		std::thread keyboardInputThread(KeyboardInputThread(world), 0);				// Keyboard input thread
		std::thread collisionCheckThread(CollisionThread(world), 0);		// Collision input thread
		
		while (true)
		{
			if (world->GetIsGameOver()) break;
			if (world->GetIsGamePaused()) continue;

			system("CLS");
			std::cout << "Use W/A/S/D to make a move (M => Add monster | P => Pause | Q => Exit)\n";

			world->TickAll();
			printGrid(world);
			printStates(world);
			nMoves++;
		}
		collisionCheckThread.join();
		keyboardInputThread.detach();
		

		// Cleaning up
		world->DeleteAllActors();
		delete world;

		printf("\n\nYou'll find one undeleted memory in the output which is from the KeyboardInputThread, since it's still waiting for user input and we cannot use join() on it. I decided to detach the thread instead (Temporary fix)\n\n");
	}

	_CrtDumpMemoryLeaks();
	return 0;
#endif
}