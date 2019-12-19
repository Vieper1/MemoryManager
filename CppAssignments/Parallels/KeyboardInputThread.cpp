#pragma once 

#include "KeyboardInputThread.h"
#include <conio.h>
#include "General/CustomComponents.h"

KeyboardInputThread::KeyboardInputThread(World*& _world)
{
	this->world_  = _world;
}

void KeyboardInputThread::operator()(int milliseconds) const
{
	char c;
	Actor *monster = nullptr;
	while ((c = _getch()) != 'q')
	{
		if (world_->GetIsGameOver())
		{
			break;
		}
		
		//moveMonsters(monsters, nMonsters);
		switch (c)
		{
		case 'w':
		case 'W':
			if (!world_->GetIsGamePaused())
				world_->GetPlayerActor()->position += Vector2{ 0,1 };
			break;
		case 'a':
		case 'A':
			if (!world_->GetIsGamePaused())
				world_->GetPlayerActor()->position += Vector2{ -1,0 };
			break;
		case 's':
		case 'S':
			if (!world_->GetIsGamePaused())
				world_->GetPlayerActor()->position += Vector2{ 0,-1 };
			break;
		case 'd':
		case 'D':
			if (!world_->GetIsGamePaused())
				world_->GetPlayerActor()->position += Vector2{ 1,0 };
			break;
		case 'M':
		case 'm':
			monster = world_->SpawnActor(nullptr, Vector2(0, 0));
			monster->AddComponent(new RandomMovementComponent(10.f));
			monster->AddComponent(new WrapAroundComponent(0, 0, COLS, ROWS));
			monster->AddComponent(new CircleColliderComponent(1.f));
			break;
		case 'x':
		case 'X':
			//removeMonster(monsters, nMonsters, 0);
			break;
		case 'p':
		case 'P':
			world_->PauseToggle();
			break;
		default:
			break;
		}
	}
	world_->EndPlay();
}
