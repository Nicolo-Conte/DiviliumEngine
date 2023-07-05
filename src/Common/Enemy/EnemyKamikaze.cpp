#include "enemy/EnemyKamikaze.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Bullet/BulletEnemyKamikaze.h"
#include "Resources/ResourceManager.h"


EnemyKamikaze::EnemyKamikaze() {
	mHealth = getEnemyInspector()->mHealthEnemyKamikaze;
	mTransform.mMoveSpeed = getEnemyInspector()->mSpeedKamikaze;
	mAttackValue = getEnemyInspector()->mAttackValueKamikaze;
	mAttackRange = getEnemyInspector()->mAttackRangeKamikaze;
	mDistanceJump = getEnemyInspector()->mDistanceJumpKamikaze;
	mDeathValue = 10;
	mJumpTimer = 0.0f;
	mKind = Kind_EnemyKamikaze;
	mName = "EnemyKamikaze";
	mTexture = "textures/Enemies/towerDefense_tile246.png";
	mTransform.mSize = glm::vec2(25, 25);
}

EnemyKamikaze::~EnemyKamikaze() {

}

void EnemyKamikaze::Attack(Entity* aEntity) {
	
	int temp = aEntity->GetKind();
	if (temp == Kind_Core || temp == Kind_Tower || temp == Kind_Turret)
	{
		if (static_cast<Building*>(aEntity)->GetHealth() - mAttackValue <= 0)
		{
			static_cast<Building*>(aEntity)->DoDamage(mAttackValue);
		}
		else
		{
			Building* building = static_cast<Building*>(aEntity);
			building->DoDamage(mAttackValue);
			OnDeath();
			
		}
	}
}


void EnemyKamikaze::DoDamage(int aDamageValue) {
	mHealth -= aDamageValue;
	if (mHealth <= 0) {
		for (float i = -1; i <= 1; i += 0.5f)
		{
			for (float j = -1; j <= 1; j += 0.5f)
			{
				if (j != 0.0f && i != 0.0f)
				{
					Entity* b = EntitySystem::CreateEntity<BulletEnemyKamikaze>();
					static_cast<Bullet*>(b)->Init(mAttackValue, 1, mTransform.mPosition, mAttackingBuilding);
					static_cast<BulletEnemyKamikaze*>(b)->SetDirection(glm::vec3(i, j, 1.0f));
					static_cast<BulletEnemyKamikaze*>(b)->SetSpeed(20.0f);
				}

			}
		}

		//TODO Shoot close buildings
		OnDeath();
	}

}

void EnemyKamikaze::MoveTo(float aDeltaTime,glm::vec2) 
{
	Building* building = static_cast<Building*>(EntitySystem::GetEntity(mAttackingBuilding));
	if (building)
	{
		mTransform.mMoveDirection = building->GetPosition() - GetPosition();
		mTransform.mMoveDirection = glm::normalize(mTransform.mMoveDirection);
	}
	mTransform.Move(aDeltaTime);
	mTransform.LookAt(mTransform.mMoveDirection);
}

