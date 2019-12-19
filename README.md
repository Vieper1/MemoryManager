# README FILE | u1261299 | Vishal Naidu


### This file describes the Project, "MemoryManager & MonsterChase" which are parts of the FinalExam







#  GAME FEATURES

The following is a list of the unique features you'd find in my MonsterChase game:


1. Some components run on separate threads | AI movement runs on the main loop inside Tick()
	- CppAssignments/Parallels/CollisionThread.cpp		=> Collision checks are here
	- Engine/General/CustomComponents.h			=> AI Random movement is here
</br>

2. Implemented an independent 'Callback' functionality for collision where the
   CollisionThread calls the OnCollisionEnter() function inside of the actor if 
   both colliding actors had the CircleCollision component attached
	- CppAssignments/Parallels/CollisionThread.cpp
</br>

3. Implemented a World class which handles
	- Engine/General/World.h
	- Engine/General/World.cpp
</br>

   (a) Actor Spawning
   (b) BeginPlay()
   (c) EventTick()
   (d) Pause
   (e) Quit
   (f) Cleanup - Deleting all actors

</br>

4. A separate thread handles KeyboardInputs, which allows the user to 
   Move, Spawn new monster while playing, Pause the game, or Quit
	- CppAssignments/Parallels/KeyboardInputThread.cpp









#  MEMORY ALLOCATOR

The MemorySystem_UnitTest() runs after the game quits.
Here's a list of all the unique features the MemoryManager has:


1. Backup Allocator => Pool Allocator
	- Engine/Memory/PoolAllocator.h
	- Engine/Memory/PoolAllocator.cpp
</br>

2. Primary Allocator Unit => Fixed Size Allocator (FSA) & There are 3 FSAs (100 - 16 byte blocks | 200 - 32 byte blocks | 400 - 96 byte blocks)
	- Engine/Memory/FixedSizeAllocator.h
	- Engine/Memory/FixedSizeAllocator.cpp
</br>

3. The MemorySystem regulates the calls to the 3 FSAs or the Pool Allocator
based on the size of the request
	- Engine/Memory/MemorySystem.h
	- Engine/Memory/MemorySystem.cpp