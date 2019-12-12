#pragma once 

#include "KeyboardInputThread.h"
#include <conio.h>

KeyboardInputThread::KeyboardInputThread(World*& _world)
{
	this->world_  = _world;
}

void KeyboardInputThread::operator()(int milliseconds) const
{
	char c;
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
			//addMonster(monsters, nMonsters);
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
