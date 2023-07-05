#include "Building\Rocket.h"
#include "rendering/renderList.h"
#include "Bullet/BulletRocket.h"
#include "Resources/ResourceManager.h"

Rocket::Rocket()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Rocket";
    mTexture = "textures/Turrets/towerDefense_tile206.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Rocket::Rocket(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Rocket";
    mTexture = "textures/Turrets/towerDefense_tile206.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Rocket::~Rocket()
{
}


void Rocket::AttackFunction()
{
    if (static_cast<Enemy*>(EntitySystem::GetEntity(mEnemyTarget))->GetHealth() > 0)
    {
        Entity* b = EntitySystem::CreateEntity<BulletRocket>();
        static_cast<BulletRocket*>(b)->Init(BuildingInspectable::GetInstance()->ATTACKROCKET, 1, mTransform.mPosition, mEnemyTarget);
    }
    else
    {
        mAttacking = false;
    }

}