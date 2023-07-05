#include "Building/Sniper.h"
#include "Enemy/EnemyWaveSystem.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"

Sniper::Sniper()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Sniper";
    mTexture = "textures/Turrets/towerDefense_tile204.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Sniper::Sniper(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Sniper";
    mTexture = "textures/Turrets/towerDefense_tile204.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Sniper::~Sniper()
{
}

void Sniper::DetectionRange()
{
    //the shortest distance will start of higher then the detection range
    float shortestDistance = 0.f;
    bool enemyFound = false;
    for (int i = 0; i < EnemyWaveSystem::GetEnemyVecSize(); i++)
    {
        Enemy* enemy = EnemyWaveSystem::getEnemy(i);
        if (enemy != nullptr)
        {
            float xd = enemy->GetPosition().x - GetPosition().x;
            float yd = enemy->GetPosition().y - GetPosition().y;
            float distance = sqrt(xd * xd + yd * yd);
            if (shortestDistance < distance && distance < mAttackRange)
            {
                shortestDistance = distance;
                mEnemyTarget = enemy->mHandle;
                mAttacking = true;
                enemyFound = true;
            }
        }
    }
    if (enemyFound == false)
    {
        mAttacking = false;
    }
}
