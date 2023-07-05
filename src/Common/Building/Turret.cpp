#include "Building\Turret.h"
#include "Enemy/Enemy.h"
#include <Entities/EntitySystem.h>
#include "Bullet/Bullet.h"
#include "Enemy/EnemyWaveSystem.h"
#include "rendering/renderList.h"
#include "Audio/Audio.h"

Turret::Turret()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Turret";
    mKind = Kind_Turret;
}

Turret::Turret(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Turret";
    mKind = Kind_Turret;
}

Turret::~Turret()
{
}

void Turret::AttackFunction()
{
    if (static_cast<Enemy*>(EntitySystem::GetEntity(mEnemyTarget))->GetHealth() > 0)
    {
        {
            divil::PlaySoundParams params;
            params.minPitch = 0.9f;
            params.maxPitch = 1.1f;
            divil::Audio::PlaySound("audio/sfx/player_shoot.wav", params);
        }

        Entity* b = EntitySystem::CreateEntity<Bullet>();
        if (b != nullptr)
            static_cast<Bullet*>(b)->Init(mAttackValue, 1, mTransform.mPosition, mEnemyTarget);
    }
    else
    {
        mAttacking = false;
    }

}

void Turret::DeathFunction()
{
    Player::GetInstance()->RemoveBuilding(this);
    EntitySystem::RemoveEntity(this);
}




void Turret::Update(float aDeltaTime) // Don't add the aDeltaTime if not using this is a better way to get rid of the warning from unused variable on ps4
{
    if (mSellTurret)
    {
        DoDamage(1);
    }

    DetectionRange();
    Entity* enemy = (EntitySystem::GetEntity(mEnemyTarget));
    if (enemy)
    {
        mTransform.LookAt(enemy->mTransform);
    }

    if (mAttacking)
    {

        mAttackTimer += aDeltaTime;
        if (mAttackTimer >= mFireRate)
        {
            mAttackTimer = 0.0f;
            AttackFunction();
        }

    }

    if (mHover && (mHealth < mMaxHealth) && mSellTurret == false)  //check if the current health is below its maximum and the cursor is hovering the turret
    {
        HealingFunction();
    }
}

void Turret::DetectionRange()
{
    //the shortest distance will start of higher then the detection range
    float shortestDistance = static_cast<float>(mAttackRange) + 10.f;
    bool enemyFound = false;
    for (int i = 0; i < EnemyWaveSystem::GetEnemyVecSize(); i++)
    {
        Enemy* enemy = EnemyWaveSystem::getEnemy(i);
        if (enemy != nullptr)
        {
            float xd = enemy->GetPosition().x - GetPosition().x;
            float yd = enemy->GetPosition().y - GetPosition().y;
            float distance = sqrt(xd * xd + yd * yd);
            if (shortestDistance > distance && distance < mAttackRange)
            {
                shortestDistance = distance;
                mEnemyTarget = enemy->mHandle;
                mAttacking = true;
                enemyFound = true;
                return;
            }
        }
    }
    if (enemyFound == false)
    {
        mAttacking = false;
    }
}



/// <summary>
/// return the angle between two points in radians
/// </summary>
/// <param name="aPosition"></param>
/// <returns></returns>


void Turret::SellTurret()
{
    if (mSellTurret == false)
    {
        mSellTurret = true;
        Player::GetInstance()->AddMoney(Player::GetInstance()->GetBuildingCost(mName));
    }
}

void Turret::DebugMenu()
{
    ImGui::Text("health : %d", static_cast<int> (GetHealth()));
}