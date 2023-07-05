#include "Building\Minigun.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"

Minigun::Minigun()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Minigun";
    mTexture = "textures/Turrets/towerDefense_tile250.png";
}

Minigun::Minigun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Minigun";
    mTexture = "textures/Turrets/towerDefense_tile250.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Minigun::~Minigun()
{
}
