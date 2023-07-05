#include "enemy/EnemyTransporter.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"

EnemyTransporter::EnemyTransporter() {
	mHealth = getEnemyInspector()->mHealthEnemyTransporter;
	mTransform.mMoveSpeed = getEnemyInspector()->mSpeedTransporter;
	mAttackValue = getEnemyInspector()->mAttackValueTransporter;
	mAttackRange = getEnemyInspector()->mAttackRangeTransporter;
	mDistanceJump = getEnemyInspector()->mDistanceJumpTransporter;
	mDeathValue = 15;
	mJumpTimer = 0.0f;
	mKind = Kind_EnemyTransporter;
	mName = "EnemyTransporter";
	mTexture= "textures/Enemies/towerDefense_tile269.png";
	mTransform.mSize = glm::vec2(25, 25);
}

EnemyTransporter::~EnemyTransporter() {

}

void EnemyTransporter::OnDeath() {

	EnemyData data;
	data.amount = 3;
	data.kind = Kind_Enemy;
	Timing time;
	time.data.push_back(data);
	time.Time = 1.5f;
	EnemyWaveSystem::SpawnEnemy(glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y), time);

	Player::GetInstance()->AddMoney(mDeathValue);
	EnemyWaveSystem::RemoveItem(mHandle);
	EntitySystem::RemoveEntity(this);

}


