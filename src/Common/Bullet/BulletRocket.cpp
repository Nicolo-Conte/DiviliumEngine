#include "Bullet\BulletRocket.h"
#include <entities/EntitySystem.h>
#include "Enemy/EnemyWaveSystem.h"
#include "rendering/renderList.h"
#include "rendering/DebugRenderer.h"
#include "enemy/Enemy.h"
#include "Resources/ResourceManager.h"
BulletRocket::BulletRocket()
{
	mAttackRange = 5;
	mAttackValue = 0;
	mTransform.mMoveSpeed = 0;
	mFounddDirection = false;
	mLifeTimer = 1.0f;
	mName = "BulletRocket";
	mExplosionRadius = BuildingInspectable::GetInstance()->ROCKETEXPLOSIONRADIUS;
	mTransform.mSize = glm::vec2(10, 10);
	mTexture = "textures/Ammo/towerDefense_tile251.png";
}

BulletRocket::~BulletRocket()
{
}


void BulletRocket::Attack(Entity*)
{
	float cx = mTransform.mPosition.x, cy = mTransform.mPosition.y;
	float r = mExplosionRadius;
	DebugRenderer::sInstance().drawCircle(mTransform.mPosition.x, mTransform.mPosition.y, mExplosionRadius, DebugColor::Black, CircleResolution::high);
	std::vector<Enemy*> EnemyVec;
	for (int i = 0; i < EnemyWaveSystem::GetEnemyVecSize(); i++)
	{
		Enemy* enemy = EnemyWaveSystem::getEnemy(i);
		if (enemy != nullptr)
		{
			float x = enemy->GetPosition().x, y = enemy->GetPosition().y;
			float distance = (powf(x - cx,2.0f) + powf(y - cy,2.0f));
			if (distance <= r * r)
			{
				EnemyVec.push_back(enemy);
			}
		}
	}
	for (int i = 0; i < EnemyVec.size(); i++)
	{
		EnemyVec[i]->DoDamage(mAttackValue);
	}
	DeathFunction();
}

