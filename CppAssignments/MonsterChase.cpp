#include "General/World.h"
#include "General/Components/Physics.h"
#include "General/Systems/Input.h"
#include "Math/Math.h"


//////////////////////////////////////////////////////////////////////////////////// Keyboard Input Event
void Keyboard_HandleActionInput(World * world, PhysicsComponent * leftPhysics, PhysicsComponent * rightPhysics)
{
	const float forceMultiplier = 1.f;
	const float rotationSpeed = -1.f;
	
	if (Input::Keyboard_IsKeyDown(87))		// UP
		leftPhysics->AddForce(Vector2::Up() * forceMultiplier);
	if (Input::Keyboard_IsKeyDown(83))		// DOWN
		leftPhysics->AddForce(Vector2::Down() * forceMultiplier);

	if (Input::Keyboard_IsKeyDown(73))		// UP
		rightPhysics->AddForce(Vector2::Up() * forceMultiplier);
	if (Input::Keyboard_IsKeyDown(75))		// DOWN
		rightPhysics->AddForce(Vector2::Down() * forceMultiplier);
	
	if (Input::Keyboard_IsKeyDown(27))
		world->EndPlay();
	//if (Keyboard_IsKeyDown(70))		// Debug Trigger
	//	playerActor->bDebugTrigger = true;
}
//////////////////////////////////////////////////////////////////////////////////// Keyboard Input Event













////////////////////////////////////////////////////////////////////////////////////// Game Logic
enum WinningPlayer
{
	None,
	LeftPlayer,
	RightPlayer
};

void EnforcePaddleBounds(SmartPtr<Actor> & leftPaddle, SmartPtr<Actor> & rightPaddle)
{
	if (Math::clampRef(leftPaddle->position.y, -200, 220))
		leftPaddle->SetVelocity(Vector2::Zero());

	if (Math::clampRef(rightPaddle->position.y, -200, 220))
		rightPaddle->SetVelocity(Vector2::Zero());
}

WinningPlayer CheckScoreAndResetBall(World * world, SmartPtr<Actor> & ball, int & leftScore, int & rightScore, const bool forceReset = false)
{
	const float hVelocityMultiplier = 300.0f;

	const float ballX = ball->position.x;
	if (forceReset)
	{
		ball->position = Vector2::Zero();
		ball->SetVelocity(Vector2(hVelocityMultiplier, 0.0f));
		return WinningPlayer::None;
	}
	else
	{
		if (ballX > 400.f)
		{
			if (++leftScore == 3)
			{
				world->EndPlay();
				return WinningPlayer::LeftPlayer;
			}
			ball->position = Vector2::Zero();
			ball->SetVelocity(Vector2(hVelocityMultiplier, 0.0f));
		}
		else if (ballX < -400.f)
		{
			if (++rightScore == 3)
			{
				world->EndPlay();
				return WinningPlayer::RightPlayer;
			}
			ball->position = Vector2::Zero();
			ball->SetVelocity(Vector2(-hVelocityMultiplier, 0.0f));
		}
		return WinningPlayer::None;
	}
}

void EnforceBallConstantVelocity(SmartPtr<Actor> & ball)
{
	const float velocityMultiplier = 400.f;
	ball->SetVelocity(ball->GetVelocity().getNormalized() * velocityMultiplier);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	{
		int LeftScore = 0;
		int RightScore = 0;
		WinningPlayer winner = WinningPlayer::None;
		
		auto world = new World(hInstance, nCmdShow);
		
		SmartPtr<Actor> leftPaddle;
		world->SpawnPrefab(leftPaddle, "Saved\\LeftPaddle.json");
		PhysicsComponent * leftPaddlePhysicsComp = leftPaddle->GetComponent<PhysicsComponent>();

		SmartPtr<Actor> rightPaddle;
		world->SpawnPrefab(rightPaddle, "Saved\\RightPaddle.json");
		PhysicsComponent * rightPaddlePhysicsComp = rightPaddle->GetComponent<PhysicsComponent>();

		SmartPtr<Actor> ball;
		world->SpawnPrefab(ball, "Saved\\Ball.json");
		


		world->LoadScene("Saved\\EditorStatePong.json");
		
		
		world->BeginPlayAll();
		CheckScoreAndResetBall(world, ball, LeftScore, RightScore, true);
		while (true)
		{
			if (world->GetIsGameOver()) break;
			if (world->GetIsGamePaused()) continue;

			// Tick
			world->TickAll();
			EnforceBallConstantVelocity(ball);
			EnforcePaddleBounds(leftPaddle, rightPaddle);
			winner = CheckScoreAndResetBall(world, ball, LeftScore, RightScore);
			if (winner != WinningPlayer::None)
				break;

			// GLib
			Keyboard_HandleActionInput(world, leftPaddlePhysicsComp, rightPaddlePhysicsComp);
			if (!world->RenderAllActors())
				break;
		}

		const size_t	lenBuffer = 127;
		char			Buffer[lenBuffer];
		sprintf_s(Buffer, lenBuffer, "\n\n\n\n\n///// Winning Player = [%s]\n\n\n\n\n", winner == LeftPlayer ? "LEFT" : "RIGHT");
		OutputDebugStringA(Buffer);

		world->Shutdown();
	}

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}
//////////////////////////////////////////////////////////////////////////////////// Game Logic









