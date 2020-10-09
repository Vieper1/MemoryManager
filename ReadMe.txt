# README FILE | u1261299 | Vishal Naidu
# GAME ENGINEERING - 1


This file describes the Project, "Pong"
That's part of the FinalExam





###########################
#  GAME FEATURES          #
###########################

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