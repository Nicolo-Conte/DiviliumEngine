#include "src\RTSPrecomp.h"
#include "Enemy/EnemySpawner.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Logger/Log.h"
#include "Building/Tower.h"
#include "Player/Player.h"
#include "Resources/ResourceManager.h"


EnemySpawner::EnemySpawner(Timing aTiming, float xPos, float yPos)
{
	mTransform.mPosition = glm::vec2(xPos, yPos);
	mTransform.GetTilePositions();
	mTiming = aTiming;
	mKind = Kind_EnemySpawner;
	mCollisionLayer = Collision::GetCollisionLayerWithName("EnemySpawner");
	mName = "EnemySpawner";
	mTexture = "textures/Enemies/DetailedComponentsCategoryA_0070_Path--------------------.png";
	mTransform.mSize = glm::vec2(25, 25);
}

void EnemySpawner::Update(float aDeltaTime)
{
	mTiming.Time -= aDeltaTime;
	if(mTiming.Time < 0)
	{
		mTime -= aDeltaTime;
		if (mTime < 0)
		{
			Spawn();
			EnemyWaveSystem::RemoveItem(mHandle);
			EntitySystem::RemoveEntity(this);
		}
	}
}

bool EnemySpawner::Spawn()
{
	mTime = mTimeInBetween;
	//make sure the enemy does not spawn on the tower
	glm::vec2 TowerPos = Player::GetInstance()->GetMainTower()->GetPosition();
	float minSpawnDistSqrt = 160;
	int tries = 5;
	//spawn the max amount we can spawn from a random data type
	for (int i = 0; i < mMaxSpawn; i++)
	{
		if (mTiming.data.empty())
			return true;

		int data = rand() % mTiming.data.size();

		int k = 0;
		//dont inifintly try finding a spawning spot
		while (k < tries)
		{
			//TODO when we are using non walkable tiles yo should not be capable of spawning there
			//create an enemy spawn position
			int sgnX = rand() % 2 * 2 - 1;
			int sgnY = rand() % 2 * 2 - 1;
			glm::vec2 pos = glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y);
			pos.x += static_cast<float>(i) * 2.f * sgnX;
			pos.y += static_cast<float>(i) * 2.f * sgnY;
			float distance = (pos.x * TowerPos.x) * (pos.x * TowerPos.x) + (pos.y * TowerPos.y) * (pos.y * TowerPos.y);
			//if the distance is bigger then the minimum required amount then spawn the enemy
			if (distance > minSpawnDistSqrt)
			{
				EnemyWaveSystem::SpawnEnemy(mTiming.data[data].kind, pos);
				//remove the enemy from the list
				mTiming.data[data].amount--;
				if (mTiming.data[data].amount <= 0)
				{
					mTiming.data.erase(mTiming.data.begin() + data);
				}
				//make the while loop stop
				k = tries + 2;
			}
			k++;
		}
	}
	return mTiming.data.empty();
}
