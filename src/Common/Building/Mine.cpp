#include "Building/Mine.h"
#include "rendering/renderList.h"
#include "Enemy/Enemy.h"
#include "Entities/EntitySystem.h"
#include "Resources/ResourceManager.h"
Mine::Mine(glm::vec2 Pos)
{
	mTransform.mPosition = Pos;
	mTransform.GetTilePositions();
	mCost = 0;
	mHealth = 100;
	mMaxHealth = mHealth;
	mName = "Mine";
	mKind = Kind_Mine;
	mEntityFlags |= EntityFlag_Ignore;
	mCollisionLayer = Collision::CreateCollisionLayerWithName("Mines");
	mTexture = "textures/Towers/250px-H4-UNSC-Landmine-Front.png";
	mTransform.mSize = glm::vec2(25, 25);
}

Mine::~Mine()
{
}

void Mine::DeathFunction()
{
}



void Mine::Update(float)
{

}

void Mine::DebugMenu()
{

}

void Mine::OnCollision(Entity* aEntity)
{
	int temp = aEntity->GetKind();
	//TODO change this to collision layers
	if (temp == Kind_Enemy || temp == Kind_EnemyKamikaze || temp == Kind_EnemySniper || temp == Kind_EnemyTransporter || temp == Kind_EnemyTank)
	{
		Enemy* e = static_cast<Enemy*> (aEntity);
		//TODO make it an explosion
		e->DoDamage(static_cast<int>(mDamage));
		EntitySystem::RemoveEntity(this);
	}
}
