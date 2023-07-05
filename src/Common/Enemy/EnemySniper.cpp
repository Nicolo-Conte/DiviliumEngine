#include "enemy/EnemySniper.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Bullet/Bullet.h"
#include "Bullet/BulletEnemySniper.h"
#include "Resources/ResourceManager.h"
#include "Audio/Audio.h"

EnemySniper::EnemySniper() {
	        mHealth = getEnemyInspector()->mHealthEnemySniper;
	mTransform.mMoveSpeed = getEnemyInspector()->mSpeedSniper;
	   mAttackValue = getEnemyInspector()->mAttackValueSniper;
	   mAttackRange = getEnemyInspector()->mAttackRangeSniper;
	 mDistanceJump = getEnemyInspector()->mDistanceJumpSniper;
	mDeathValue = 10;
	mJumpTimer = 0.0f;
	mKind = Kind_EnemySniper;
	mName = "EnemySniper";
	mAttackTimer = 1.0f;
	mAttackDelay = 1.0f;
	mTexture = "textures/Enemies/towerDefense_tile247.png";
	mTransform.mSize = glm::vec2(25, 25);
}

EnemySniper::~EnemySniper() {

}
void EnemySniper::Update(float aDT)
{
	Enemy::Update(aDT);
	mAttackTimer -= aDT;
}
void EnemySniper::Attack(Entity* aEntity) {
	
	if (mAttackTimer <= 0.0f)
	{
        {
            divil::PlaySoundParams params;
            params.minPitch = 1.5f;
            params.maxPitch = 2.0f;
            divil::Audio::PlaySound("audio/sfx/player_shoot.wav", params);
        }

		Entity* b = EntitySystem::CreateEntity<BulletEnemySniper>();
		static_cast<Bullet*>(b)->Init(mAttackValue, 1, mTransform.mPosition, aEntity->mHandle);
		mAttackTimer = mAttackDelay;
	}
	
}
