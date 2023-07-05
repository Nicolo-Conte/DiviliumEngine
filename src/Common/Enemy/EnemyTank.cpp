#include "enemy/EnemyTank.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"

EnemyTank::EnemyTank() {
	mHealth = getEnemyInspector()->mHealthEnemyTank;
	mTransform.mMoveSpeed = getEnemyInspector()->mSpeedTank;
	mAttackValue = getEnemyInspector()->mAttackValueTank;
	mAttackRange = getEnemyInspector()->mAttackRangeTank;
	mDistanceJump = getEnemyInspector()->mDistanceJumpTank;
	mDeathValue = 10;
	mJumpTimer = 0.0f;
	mKind = Kind_EnemyTank;
	mName = "EnemyTank";
	mTexture = "textures/Enemies/towerDefense_tile268.png";
	mTransform.mSize = glm::vec2(25, 25);
}

EnemyTank::~EnemyTank() {

}