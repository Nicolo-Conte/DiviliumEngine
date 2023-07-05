#include "Player\CoreManager.h"
#include "Entities/EntitySystem.h"
#include "Building/Core.h"
#include "Player/Player.h"
#include "GameClass/GameClass.h"
#include "ResearchSystem/ResearchSystem.h"
#include "Entities/Transform.h"
#include "Building/Tower.h"
CoreManager::CoreManager()
{
	clear();
}

void CoreManager::clear()
{
	mTotalCores = 0;

	for (int i = 0; i < mCoresActive.size(); i++)
	{
		Building* building = static_cast<Building*> (EntitySystem::GetEntity(mCoresActive[i]));
		Player::GetInstance()->RemoveBuilding(building);
		EntitySystem::RemoveEntity(building);
	}
	mCoresActive.clear();
}

void CoreManager::Start()
{
	mTotalCores++;
	mLeftToSpawn = mTotalCores;
	
	if (mTotalCores == 1)
		mMax = 1;
	else if (mTotalCores < 5)
		mMax = 2;
	else
		mMax = 3;

	Spawn();
}

void CoreManager::Spawn()
{
	int Max = glm::min(mMax - static_cast<int>(mCoresActive.size()),mLeftToSpawn);
	glm::vec2 base = Player::GetInstance()->GetMainTower()->GetPosition();
	for (int i = 0; i < Max; i++)
	{
		glm::vec2 Pos;
		Pos.x = base.x + (static_cast<float>(rand()) / static_cast<float> (RAND_MAX)) * ((mMaxDistance.x - mMinDistance.x) + mMinDistance.x) * ((rand() % 2) * 2 - 1);
		Pos.y = base.y + (static_cast<float>(rand()) / static_cast<float> (RAND_MAX)) * ((mMaxDistance.y - mMinDistance.y) + mMinDistance.y) * ((rand() % 2) * 2 - 1);
		Place(Pos);
	}
}

void CoreManager::Place(glm::vec2 Pos)
{
	Entity* entity = EntitySystem::CreateEntity<Core>(this, Pos);
	mCoresActive.push_back(entity->mHandle);
	mLeftToSpawn--;
	Player::GetInstance()->AddBuilding(static_cast<Building*>(entity));
}

void CoreManager::DeleteCore(Core* aCore)
{
	EntityHandle handle = aCore->mHandle;
	for (int i = 0; i < mCoresActive.size(); i++)
	{
		if (mCoresActive[i].generation == handle.generation 
			&& handle.index == mCoresActive[i].index)
		{
			mCoresActive.erase(mCoresActive.begin() + i );
			break;
		}
	}
	Player::GetInstance()->RemoveBuilding(aCore);
	EntitySystem::RemoveEntity(aCore);

	if (mLeftToSpawn <= 0 && mCoresActive.empty())
		GameClass::GetInstance()->getResearchSystem()->CoresComplete();
	else
		Spawn();
}
