#include "src\RTSPrecomp.h"
#include "Bullet\BulletEnemySniper.h"
#include "enemy/Enemy.h"
#include "Building/Building.h"
#include "Building/Tower.h"
#include <entities/EntitySystem.h>
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"

BulletEnemySniper::BulletEnemySniper()
{
	mAttackRange = 5;
	mAttackValue = 0;
	mTransform.mMoveSpeed =	0;
	mFounddDirection = false;
	mLifeTimer = 1.0f;
	mName = "BulletEnemySniper";
	mCollisionLayer = Collision::CreateCollisionLayerWithName("EnemyBullets");
	mTransform.mSize = glm::vec2(10, 10);
	mTexture = "textures/Ammo/towerDefense_tile275.png";
}

BulletEnemySniper::~BulletEnemySniper()
{
}


void BulletEnemySniper::Attack(Entity* aEntity) {
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

bool BulletEnemySniper::CheckDistance() {
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

void BulletEnemySniper::Update(float aDeltaTime) {
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
