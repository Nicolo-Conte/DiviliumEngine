#include "Building\Shotgun.h"
#include "rendering/renderList.h"
#include "bullet/Bullet.h"
#include "Resources/ResourceManager.h"
#include "Audio/Audio.h"

Shotgun::Shotgun()
{
    mAttackRange = 0;
    mAttackValue = 0;
    mFireRate = 0.f;
    mAttacking = false;
    mAttackTimer = 0.f;
    mEnemyTarget = EntityHandle();
    mName = "Shotgun";
    mTransform.mSize = glm::vec2(30, 30);
}

Shotgun::Shotgun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth)
{
    mAttackRange = aAttackRange;
    mAttackValue = aAttackValue;
    mFireRate = aFireRate;
    mHealth = aHealth;
    mMaxHealth = mHealth;
    mAttackTimer = 0.f;
    mAttacking = false;
    mEnemyTarget = EntityHandle();
    mName = "Shotgun";
    mTransform.mRotation = 0;
    mTexture = "textures/Turrets/towerDefense_tile226.png";
    mTransform.mSize = glm::vec2(30, 30);
}

Shotgun::~Shotgun()
{
}

void Shotgun::AttackFunction()
{
    glm::vec2 Up = glm::vec2(0, 1);
    if (static_cast<Enemy*>(EntitySystem::GetEntity(mEnemyTarget))->GetHealth() > 0)
    {
        {
            divil::PlaySoundParams params;
            params.minPitch = 0.6f;
            params.maxPitch = 0.8f;
            divil::Audio::PlaySound("audio/sfx/ufo_shoot.wav", params);
        }

        for (int i = 0; i <= 10; i++)
        {
            float random1 = static_cast <float>(rand()) / static_cast <float> (RAND_MAX ) * 90.f - 45.f;
            //float random2 = static_cast <float>(rand()) / static_cast <float> (RAND_MAX / 1.f);
            glm::vec3 direction;
            direction.z = mTransform.mRotation;
            float angle = direction.z + 180.f;
            direction.x = (COS((angle + random1)) * Up.x) - (SIN((angle + random1)) * Up.y);
            direction.y = (SIN((angle + random1)) * Up.x) + (COS((angle + random1)) * Up.y);
            Entity* b = EntitySystem::CreateEntity<Bullet>();
            static_cast<Bullet*>(b)->Init(BuildingInspectable::GetInstance()->ATTACKSHOTGUN, 1, mTransform.mPosition, mEnemyTarget, direction);
        }
    }
    else
    {
        mAttacking = false;
    }

}