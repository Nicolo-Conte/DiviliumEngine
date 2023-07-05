#include "enemy/EnemySoldier.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"


EnemySoldier::EnemySoldier() {
	mHealth = getEnemyInspector()->mHealthEnemySoldier;
	mTransform.mMoveSpeed = getEnemyInspector()->mSpeedSoldier;
	mAttackValue = getEnemyInspector()->mAttackValueSoldier;
	mAttackRange = getEnemyInspector()->mAttackRangeSoldier;
	mDistanceJump = getEnemyInspector()->mDistanceJumpSoldier;
	mDeathValue = 10;
	mJumpTimer = 0.0f;
	mKind = Kind_Enemy;
	mName = "EnemySoldier";
	mTexture = "textures/Enemies/towerDefense_tile248.png";
	mTransform.mSize = glm::vec2(25, 25);
}

EnemySoldier::~EnemySoldier() {

}
