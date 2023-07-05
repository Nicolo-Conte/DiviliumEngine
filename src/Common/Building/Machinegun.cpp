#include "Building/Machinegun.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"


Machinegun::Machinegun()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Machinegun";
    mTexture = "textures/Turrets/towerDefense_tile249.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Machinegun::Machinegun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Machinegun";
    mTexture = "textures/Turrets/towerDefense_tile249.png";
    mTransform.mSize = glm::vec2(30,30);
}

Machinegun::~Machinegun()
{
}

