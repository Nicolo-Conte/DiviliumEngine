#include "src\RTSPrecomp.h"
#include "Bullet\BulletEnemyKamikaze.h"
#include "Building/Building.h"
#include <entities/EntitySystem.h>
#include "rendering/renderList.h"
#include "GameClass/GameClass.h"

#include "Resources/ResourceManager.h"
BulletEnemyKamikaze::BulletEnemyKamikaze()
{
	mAttackRange = 5;
	mAttackValue = 0;
	mTransform.mMoveSpeed = 0;
	mFounddDirection = false;
	mLifeTimer = 1.0f;
	mName = "BulletEnemyKamikze";
	mTransform.mDirection = glm::vec2(0.0f);
	mCollisionLayer = Collision::CreateCollisionLayerWithName("EnemyBullets");
	mTexture = "textures/Ammo/towerDefense_tile274.png";
	mTransform.mSize = glm::vec2(10,10);

}

BulletEnemyKamikaze::~BulletEnemyKamikaze()
{

}

void BulletEnemyKamikaze::Attack(Entity* aEntity) {
	if (static_cast<Building*>(aEntity)->GetHealth() - mAttackValue <= 0)
	{
		static_cast<Building*>(aEntity)->DoDamage(mAttackValue);
	}
	else
	{
		static_cast<Building*>(aEntity)->DoDamage(mAttackValue);
		DeathFunction();
	}


}


bool BulletEnemyKamikaze::CheckDistance() {
	Building* building = static_cast<Building*>(EntitySystem::GetEntity(mTarget));
	if (building)
	{
		float xd = building->GetPosition().x - GetPosition().x;
		float yd = building->GetPosition().y - GetPosition().y;
		float distance = sqrt(xd * xd + yd * yd);
		if (distance <= mAttackRange)
		{
			return true;
		}
	}
	return false;
}

void BulletEnemyKamikaze::Update(float aDeltaTime) {
	mLifeTimer -= aDeltaTime;

		Building* building = static_cast<Building*>(EntitySystem::GetEntity(mTarget));
		if (building)
		{
		mTransform.MoveTo(building->GetPosition(), aDeltaTime);
		}
		else
		{
			DeathFunction();
			return;
		}

	if (mLifeTimer <= 0.0f)
	{
		DeathFunction();
	}
}

void BulletEnemyKamikaze::OnCollision(Entity* aEntity) {
	const char* val1 = "Turret";
	const char* val2 = "Tower";
	const char* val3 = "Sniper";
	const char* val4 = "Shotgun";
	const char* val5 = "Rocket";
	const char* val6 = "Machinegun";
	const char* val7 = "Rocket";

	/*switch (EntitySystem::GetEntity(aEntity->mHandle)->GetKind())
	{
		case
	default:
		break;
	}*/
	if (aEntity->mName == val1  || aEntity->mName == val3 || aEntity->mName == val4 || aEntity->mName == val5 || aEntity->mName == val6 || aEntity->mName == val7)
	{
		static_cast<Building*>(EntitySystem::GetEntity(aEntity->mHandle))->DoDamage(mAttackValue);
		DeathFunction();
	}
	if (aEntity->mName == val2)
	{
		if (static_cast<Building*>(EntitySystem::GetEntity(mTarget))->GetHealth() - mAttackValue <= 0)
		{
			static_cast<Building*>(EntitySystem::GetEntity(mTarget))->DoDamage(mAttackValue);
		}
		else
		{
			static_cast<Building*>(EntitySystem::GetEntity(mTarget))->DoDamage(mAttackValue);
			DeathFunction();
		}
	}

}