#pragma once 

#include "CollisionSystem.h"
#include "../Components/BoxCollider.h"
#include "../Components/Physics.h"
#include "../Actor.h"


CollisionSystem::CollisionSystem(World * _world)
{
	this->world = _world;
}

void CollisionSystem::operator()(int milliseconds)
{
	while (!world->GetIsGameOver())
	{
		if (world->GetGameTime() < 1.0f) continue;
		//RunCheck();
	}
}

void CollisionSystem::Run()
{
	// -------------------------------------------------- DETECTION
	collisionDataArray.clear();
	
	if (!world->GetIsGamePaused())
	{
		std::vector<WeakPtr<Actor>> & tempActors = world->GetAllWeakActorRefs();
		//world->CurrentActorsMutex.lock();
		/*for (auto actor : tempActors)
		{
			if (!actor)
				printf("Viper");
			
			SmartPtr<Actor> smartRef;
			actor.acquire(smartRef);
			actor.acquire(smartRef);
			if (actor.acquire(smartRef))
				printf("Viper");
			if (!smartRef)
				printf("Viper");
		}*/
		//world->CurrentActorsMutex.unlock();
		//return;	// TESTING
		//world->CurrentActorsMutex.lock();
		const float DeltaSeconds = world->GetDeltaSeconds();
		std::vector<WeakPtr<Actor>> & actors = world->GetAllWeakActorRefs();
		const size_t actorsLen = actors.size();
		/*if (actorsLen < 1)
			continue;*/

		for (unsigned int i = 0; i < actorsLen - 1; i++)
		{
			SmartPtr<Actor> myActor;
			actors[i].acquire(myActor);
			if (!myActor) continue;
			const BoxColliderComponent * myCollider = myActor->GetComponent<BoxColliderComponent>();
			if (!myCollider) continue;

			for (int j = i + 1; j < actorsLen; j++)
			{
				SmartPtr<Actor> otherActor;
				actors[j].acquire(otherActor);
				if (!otherActor || otherActor == myActor) continue;

				const BoxColliderComponent * otherCollider = otherActor->GetComponent<BoxColliderComponent>();
				if (!otherCollider) continue;




				// COLLISION LOGIC
				float tMyEnter = 9999;
				float tOtherEnter = 9999;
				float tMyExit = 9999;
				float tOtherExit = 9999;

				const AABB myBounds = myCollider->getBounds();
				const AABB otherBounds = otherCollider->getBounds();


				const Matrix4 mOtherRot = Matrix4::ZRotation(otherActor->zRotation);
				const Matrix4 mOtherTrans = Matrix4::Translation(otherActor->position);
				const Matrix4 mOtherToWorld = mOtherTrans * mOtherRot;
				const Matrix4 mWorldToOther = mOtherToWorld.inverse();

				const Matrix4 mMyRot = Matrix4::ZRotation(myActor->zRotation);
				const Matrix4 mMyTrans = Matrix4::Translation(myActor->position);
				const Matrix4 mMyToWorld = mMyTrans * mMyRot;
				const Matrix4 mWorldToMy = mMyToWorld.inverse();


				const Matrix4 mOtherToMy = mWorldToMy * mOtherToWorld;
				const Matrix4 mMyToOther = mWorldToOther * mMyToWorld;

				const Vector4 myVelocity = myActor->GetVelocity();
				const Vector4 otherVelocity = otherActor->GetVelocity();
				const Vector4 myWorldRelativeVelocity = myVelocity - otherVelocity;
				const Vector4 otherWorldRelativeVelocity = myWorldRelativeVelocity.getInverse();




				//////////////////////////////////////////////////////////////////////////////////////////////////// MY TEST
				const Vector4 otherCenterInMy = mOtherToMy * Vector4(otherBounds.Center, 1.f);

				const Vector4 otherExtentsXInMy = mOtherToMy * Vector4(otherBounds.Extents.x / 2, 0, 0, 0);
				const Vector4 otherExtentsYInMy = mOtherToMy * Vector4(0, otherBounds.Extents.y / 2, 0, 0);

				const float otherProjectionXOntoMyEnd = fabs(otherExtentsXInMy.x) + fabs(otherExtentsYInMy.x);
				const float otherProjectionYOntoMyEnd = fabs(otherExtentsXInMy.y) + fabs(otherExtentsYInMy.y);


				// Velocity Based Detection
				const Vector4 otherLocalRelativeVelocity = mMyRot * otherWorldRelativeVelocity;

				Vector4 myHitNormal;
				float myNormalTime = 0.f;


				float tMyH0 = -1;
				float tMyH1 = -1;
				bool bMyHSkip = false;
				if (Math::IsNearlyZero(otherLocalRelativeVelocity.x))
					bMyHSkip = true;
				else
				{
					const float tR = ((myBounds.Center.x - (myBounds.Extents.x / 2)) - (otherCenterInMy.x + otherProjectionXOntoMyEnd)) / otherLocalRelativeVelocity.x;
					const float tL = ((myBounds.Center.x + (myBounds.Extents.x / 2)) - (otherCenterInMy.x - otherProjectionXOntoMyEnd)) / otherLocalRelativeVelocity.x;
					if (tR < tL)
					{
						myHitNormal = Vector2::Right();
						myNormalTime = tR;
					}
					else
					{
						myHitNormal = Vector2::Left();
						myNormalTime = tR;
					}
					tMyH0 = min(tR, tL);
					tMyH1 = max(tR, tL);
				}


				float tMyV0 = -1;
				float tMyV1 = -1;
				bool bMyVSkip = false;
				if (Math::IsNearlyZero(otherLocalRelativeVelocity.y))
					bMyVSkip = true;
				else
				{
					const float tU = ((myBounds.Center.y - (myBounds.Extents.y / 2)) - (otherCenterInMy.y + otherProjectionYOntoMyEnd)) / otherLocalRelativeVelocity.y;
					const float tD = ((myBounds.Center.y + (myBounds.Extents.y / 2)) - (otherCenterInMy.y - otherProjectionYOntoMyEnd)) / otherLocalRelativeVelocity.y;
					if (tU < tD && tU < myNormalTime)
					{
						myHitNormal = Vector2::Up();
						myNormalTime = tU;
					}
					if (tD < tU && tD < myNormalTime)
					{
						myHitNormal = Vector2::Down();
						myNormalTime = tD;
					}
					tMyV0 = min(tU, tD);
					tMyV1 = max(tU, tD);
				}




				float tMyInMax = -1;
				float tMyOutMin = -1;
				int tMyModeIndicator = -1;
				bool bMySkip = false;

				if (!bMyHSkip && !bMyVSkip)
				{
					tMyInMax = max(tMyH0, tMyV0);
					tMyOutMin = min(tMyH1, tMyV1);
					if (tMyInMax < tMyOutMin)
					{
						tMyEnter = tMyInMax;
						tMyExit = tMyOutMin;
					}
					tMyModeIndicator = 0;
				}
				if (!bMyHSkip/* && bH*/ && bMyVSkip)
				{
					tMyInMax = tMyH0;
					tMyOutMin = tMyH1;
					tMyEnter = tMyInMax;
					tMyExit = tMyOutMin;
					tMyModeIndicator = 1;
				}
				if (bMyHSkip && !bMyVSkip/* && bV*/)
				{
					tMyInMax = tMyV0;
					tMyOutMin = tMyV1;
					tMyEnter = tMyInMax;
					tMyExit = tMyOutMin;
					tMyModeIndicator = 2;
				}
				if (bMyHSkip && bMyVSkip)
				{
					bMySkip = true;
				}

				//myHitNormal = mMyToWorld * Vector4(myHitNormal);
				//////////////////////////////////////////////////////////////////////////////////////////////////// MY TEST





				//////////////////////////////////////////////////////////////////////////////////////////////////// OTHER TEST
				const Vector4 myCenterInOther = mMyToOther * Vector4(myBounds.Center, 1.f);

				const Vector4 myExtentsXInOther = mMyToOther * Vector4(myBounds.Extents.x / 2, 0, 0, 0);
				const Vector4 myExtentsYInOther = mMyToOther * Vector4(0, myBounds.Extents.y / 2, 0, 0);

				const float myProjectionXOntoOtherEnd = fabs(myExtentsXInOther.x) + fabs(myExtentsYInOther.x);
				const float myProjectionYOntoOtherEnd = fabs(myExtentsXInOther.y) + fabs(myExtentsYInOther.y);


				// Velocity Based Detection
				const Vector4 myLocalRelativeVelocity = mOtherRot * myWorldRelativeVelocity;

				float tOtherH0 = -1;
				float tOtherH1 = -1;
				bool bOtherHSkip = false;
				if (Math::IsNearlyZero(myLocalRelativeVelocity.x))
					bOtherHSkip = true;
				else
				{
					const float tR = ((otherBounds.Center.x - (otherBounds.Extents.x / 2)) - (myCenterInOther.x + myProjectionXOntoOtherEnd)) / myLocalRelativeVelocity.x;
					const float tL = ((otherBounds.Center.x + (otherBounds.Extents.x / 2)) - (myCenterInOther.x - myProjectionXOntoOtherEnd)) / myLocalRelativeVelocity.x;
					tOtherH0 = min(tR, tL);
					tOtherH1 = max(tR, tL);
				}


				float tOtherV0 = -1;
				float tOtherV1 = -1;
				bool bOtherVSkip = false;
				if (Math::IsNearlyZero(myLocalRelativeVelocity.y))
					bOtherVSkip = true;
				else
				{
					const float tU = ((otherBounds.Center.y - (otherBounds.Extents.y / 2)) - (myCenterInOther.y + myProjectionYOntoOtherEnd)) / myLocalRelativeVelocity.y;
					const float tD = ((otherBounds.Center.y + (otherBounds.Extents.y / 2)) - (myCenterInOther.y - myProjectionYOntoOtherEnd)) / myLocalRelativeVelocity.y;
					tOtherV0 = min(tU, tD);
					tOtherV1 = max(tU, tD);
				}




				float tOtherInMax = -1;
				float tOtherOutMin = -1;
				int tOtherModeIndicator = -1;
				bool bOtherSkip = false;

				if (!bOtherHSkip && !bOtherVSkip)
				{
					tOtherInMax = max(tOtherH0, tOtherV0);
					tOtherOutMin = min(tOtherH1, tOtherV1);
					if (tOtherInMax < tOtherOutMin)
					{
						tOtherEnter = tOtherInMax;
						tOtherExit = tOtherOutMin;
					}
					tOtherModeIndicator = 0;
				}
				if (!bOtherHSkip && bOtherVSkip)
				{
					tOtherInMax = tOtherH0;
					tOtherOutMin = tOtherH1;
					tOtherEnter = tOtherInMax;
					tOtherExit = tOtherOutMin;
					tOtherModeIndicator = 1;
				}
				if (bOtherHSkip && !bOtherVSkip)
				{
					tOtherInMax = tOtherV0;
					tOtherOutMin = tOtherV1;
					tOtherEnter = tOtherInMax;
					tOtherExit = tOtherOutMin;
					tOtherModeIndicator = 2;
				}
				if (bOtherHSkip && bOtherVSkip)
				{
					bOtherSkip = true;
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////// OTHER TEST



				if (bMySkip && bOtherSkip)
					continue;

				if (myActor->bDebugTrigger)
					myActor->bDebugTrigger = false;


				// FINALIZE
				const float tFinalEnter = max(tMyEnter, tOtherEnter);
				const float tFinalExit = min(tMyExit, tOtherExit);
				if (tMyEnter < 0 && tMyExit > 0 && tOtherEnter < 0 && tOtherExit > 0)
				{
					if (myActor->GetMobility() == EMobility::Moveable) myActor->OnCollisionStay(otherActor);
					if (otherActor->GetMobility() == EMobility::Moveable) otherActor->OnCollisionStay(myActor);

					if (myActor->bDestroyedOnHit) world->DeleteActor(myActor);
					if (otherActor->bDestroyedOnHit) world->DeleteActor(otherActor);
				}

				if (tMyEnter > 0 && tMyEnter < DeltaSeconds && tMyExit > 0 && tOtherEnter > 0 && tOtherEnter < DeltaSeconds && tOtherExit > 0)
				{
					collisionDataArray.push_back(CollisionData{ myActor, otherActor, tFinalEnter });
				}
			}
		}








		// -------------------------------------------------- RESOLUTION
		float tLeftInFrame = DeltaSeconds;
		while (!collisionDataArray.empty() && tLeftInFrame > 0)
		{
			unsigned int dataIndex = 0;
			for (unsigned int i = 1; i < collisionDataArray.size(); i++)
				if (collisionDataArray[i].enterTime < collisionDataArray[dataIndex].enterTime)
					dataIndex = i;


			tLeftInFrame -= collisionDataArray[dataIndex].enterTime;
			for (auto weakRef : actors)
			{
				SmartPtr<Actor> actor;
				weakRef.acquire(actor);
				if (actor->GetMobility() == EMobility::Moveable) actor->position += actor->GetVelocity() * collisionDataArray[dataIndex].enterTime;
			}


			SmartPtr<Actor> actor1 = collisionDataArray[dataIndex].actor1;
			SmartPtr<Actor> actor2 = collisionDataArray[dataIndex].actor2;

			const float actor1Speed = actor1->GetVelocity().getMagnitude();
			const float actor2Speed = actor2->GetVelocity().getMagnitude();
			if (actor1->GetMobility() == EMobility::Moveable) actor1->SetVelocity((actor1->position - actor2->position).getNormalized() * ((actor2Speed + actor1Speed) / 2));
			if (actor2->GetMobility() == EMobility::Moveable) actor2->SetVelocity((actor2->position - actor1->position).getNormalized() * ((actor2Speed + actor1Speed) / 2));

			if (actor1->bDestroyedOnHit) world->DeleteActor(actor1);
			if (actor2->bDestroyedOnHit) world->DeleteActor(actor2);

			collisionDataArray.erase(collisionDataArray.begin() + dataIndex);
		}
		//world->CurrentActorsMutex.unlock();
	}
}