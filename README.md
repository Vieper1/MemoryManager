# Custom Game Engine
This is a game engine I built from scratch on C++ </br>
It includes the following systems:</br>
  1. A <b>Linear Memory-Pool</b> Allocator
  2. A <b>Fixed Size</b> Allocator
  3. A <b>Custom Smart-Pointer</b> System
  4. A <b>2D-Physics</b> Engine
  5. A <b>Box-2D Collision</b> Engine
  6. Unreal's <b>Actor Hierarchy</b> System (To build Player & NPC actors from)
  7. Dynamic <b>Component Attachment</b> System (E.g. Keyboard Inputs, Box Collider, Circle Collider)

</br>

#  MEMORY ALLOCATOR
Here's a list of all the unique features the MemoryManager has:

1. Primary Allocator Unit => Created a <b>Fixed Size Allocator (FSA)</b> & included 3 instances of it to handle allocations of different sizes (100 - <b>16 byte blocks</b> | 200 - <b>32 byte blocks</b> | 400 - <b>96 byte blocks</b>)
	- <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/FixedSizeAllocator.h">Header</a> | <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/FixedSizeAllocator.cpp">C++</a>
	- The FSA is able to store anything <b>less than its BLOCK SIZE</b> to be stored in the <b>first free block</b>
	- This one's <b>highly efficient</b> because of the <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/BitArray.cpp">BIT-ARRAY State-Keeper</a>

![Fixed Size Allocator](FixedSizeAllocator.png)

</br>

2. Backup Allocator => <b>Pool Allocator</b>
	- <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/PoolAllocator.h">Header</a> | <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/PoolAllocator.cpp">C++</a>
	- The <b>Memory-Pool</b> style allocator allocates blocks <b>linearly</b> regardless of their size
	- There's a <b>ratio of the TOTAL POOL SIZE</b> that stores <b>metadata</b> for each of the allocated blocks

![Memory Pool Allocator](MemoryPoolAllocator.png)

</br>


3. The MemorySystem regulates the calls to the 3 FSAs or the Pool Allocator
based on the size of the request
	- <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/MemorySystem.h">Header</a> | <a href="https://github.com/Vieper1/MemoryManager/blob/part2/Engine/Memory/MemorySystem.cpp">C++</a>
	- This system <b>receives the allocation calls</b> and invokes the <b>respective allocator</b>




#  GAME FEATURES

How to play the game:
1. Move the LEFT paddle up/down using W/S
2. Move the RIGHT paddle up/down using I/K
3. If the Ball moves past the left paddle, the right player's score increases by 1 (And vice versa)
4. The first player to reach a score of 3 wins the game
5. The windows shuts itself down to close the game, printing the win-state in the output log

NOTE:	The walls comprise of white square bricks placed close to each other. Each reflect the ball in a random direction to give a sense of randomness to the game
	(Not really an interesting feature, but I just had in there since it makes it fun)



The following is a list of features you'd find in my Pong game:

1. I noticed GLib's input handler gives you callbacks with each button press/release, so I used that to update a std::vector<uints> in Input.h every time there's a key callback
   This let me have a "Keyboard_IsKeyDown(uint Button_ID)" function similar to that in Unity

	- Engine/Systems/Input.h/cpp				=> key data handled here
	- CppAssignments/MonsterChase.cpp			=> usage of Keyboard_IsKeyDown(uint Button_ID)


2. Uses a SmartPtr/WeakPtr system
	- Only the property "std::vector<SmartPtr<Actor>> actors" in World holds references to the actors
	- There's a WeakPtr copy of the actors, which is used to distribute weak references to observer systems
	- Systems like Components (Physics, Box/CircleColliders, Follow), CollisionSystem, Input depend on the weak referernces and try to acquire full refs to work with


3. Implemented a World class which handles
	- Engine/General/World.h
	- Engine/General/World.cpp

   a. Actor Spawning (With and without JSON for prefab)
   b. BeginPlay()
   c. EventTick()
   d. Pause
   e. Quit
   f. Cleanup - Deleting all actors | Stop GLib Service

4. Physics Components
	- Each actor with an attached PhysicsComponent will have simulated motion with forces leading to linear acceleration
	- Drag forces included
	- X/Y constraints to lock movement in one/both axes
	- AddForce() function accumulates impending forces before applying them together in one Tick()

5. Math
	- Math.h provides some basic functionality like randomRange(), clamp(), isNearlyZero/Equal()
	- Vector classes for sizes 2, 3, 4
	- Matrix classes for sizes 3, 4
	- Matrix classes with transform matrices for translation, rotation and scaling

6. Collision
	- Uses the Swept-Axis collision check method to identify impending collisions
	- Calls on the actors' OnCollisionStay() and OnHit() functions when impending collision is found
	- Forwards all objects to the latest resolved impending collision time
