// CppAssignments.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>
#include "General/World.h"
#include "String/String.h"
#include <thread>
#include <algorithm>
#include "Memory/MemorySystem.h"
#include "General/CustomComponents.h"
#include "Parallels/KeyboardInputThread.h"
#include "Parallels/CollisionThread.h"
#include <conio.h>


////////////////////////////////////////////////////////////////////// Declarations
char grid[ROWS][COLS];













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
		else
			printf(")");
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

















#define GAME
//#define UNIT_TEST






// Declare UnitTest
using namespace HeapManagerProxy;
bool MemorySystem_UnitTest();

int main()
{
	InitializeMemorySystem();

	
#ifdef GAME
	{
		EnableDebugMessages(false);

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
		player->AddComponent(new CircleColliderComponent(1.f));		// COMMENT OUT THIS LINE TO REMOVE PLAYER COLLISION
		world->SetPlayerActor(player);



		// Create Single Monster
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
		std::thread collisionCheckThread(CollisionThread(world), 0);					// Collision input thread

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

		printf("\n\nPress any key to continue...");
		_getch();
		system("CLS");

		EnableDebugMessages(true);
	}
#endif




	
#ifdef UNIT_TEST
	const bool success = MemorySystem_UnitTest();
	assert(success);
#endif





	


	DestroyMemorySystem();
	
#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	
	return 0;
}














// MACRO
#define malloc(x) CustomMalloc(x)
#define free(x) CustomFree(x)



// UNIT TEST
bool MemorySystem_UnitTest()
{
	const size_t maxAllocations = 10 * 1024;
	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	size_t totalAllocated = 0;

	// reserve space in AllocatedAddresses for the maximum number of allocation attempts
	// prevents new returning null when std::vector expands the underlying array
	AllocatedAddresses.reserve(10 * 1024);

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t		maxTestAllocationSize = 1024;

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		void * pPtr = malloc(sizeAlloc);

		// if allocation failed see if garbage collecting will create a large enough block
		if (pPtr == nullptr)
		{
			Collect();

			pPtr = malloc(sizeAlloc);

			// if not we're done. go on to cleanup phase of test
			if (pPtr == nullptr)
				break;
		}

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		totalAllocated += sizeAlloc;

		// randomly free and/or garbage collect during allocation phase
		const unsigned int freeAboutEvery = 0x07;
		const unsigned int garbageCollectAboutEvery = 0x07;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void * pPtrToFree = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			free(pPtrToFree);
			numFrees++;
		}
		else if ((rand() % garbageCollectAboutEvery) == 0)
		{
			Collect();

			numCollects++;
		}

	}
	while (numAllocs < maxAllocations);

	// now free those blocks in a random order
	if (!AllocatedAddresses.empty())
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while (!AllocatedAddresses.empty())
		{
			void * pPtrToFree = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			delete[] pPtrToFree;
		}

		// do garbage collection
		Collect();
		// our heap should be one single block, all the memory it started with

		// do a large test allocation to see if garbage collection worked
		void * pPtr = malloc(totalAllocated / 2);

		if (pPtr)
		{
			free(pPtr);
		}
		else
		{
			// something failed
			return false;
		}
	}
	else
	{
		return false;
	}

	// this new [] / delete [] pair should run through your allocator
	char * pNewTest = new char[1024];

	delete[] pNewTest;

	// we succeeded
	return true;
}