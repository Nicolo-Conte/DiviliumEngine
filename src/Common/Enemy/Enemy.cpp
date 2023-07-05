#include "Audio/Audio.h"
#include "DebugUI/ImGuiHandler.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "Resources/ResourceManager.h"
#include "enemy/enemy.h"
#include "rendering/renderList.h"

Enemy::Enemy() {
	mHealth = 10;
	mTransform.mMoveSpeed = 400.0f;
	mAttackValue = 1;
	mAttackRange = 50.0f;
	mDeathValue = 10;
	mMoving = false;
	mAttacking = false;
	mDistanceJump = 5.0f;
	mTransform.mMoveDirection = glm::vec2(0.0f, 0.0f);
	mMoveOffset = 20.02f;
	mJumpTimer = 0.0f;
	mJumpLeft = true;
	mKind = Kind_Enemy;
	mAttackingBuilding = EntityHandle();
	mName = "Enemy";
	mCollisionLayer = Collision::CreateCollisionLayerWithName("Enemies");
	mTexture = "textures/Enemies/towerDefense_tile245.png";
	mTransform.mSize = glm::vec2(25, 25);

	if (!mParticleSystem)
	{
		mParticleSystem = new divil::ParticleSystem(256, { glm::vec2(0, 500.0f) });
	}
}

Enemy::~Enemy() {

}

void Enemy::OnDeath() {

	{
		divil::PlaySoundParams params;
		params.minPitch = 0.9f;
		params.maxPitch = 1.1f;
		divil::Audio::PlaySound("audio/sfx/ufo_die.wav", params);
	}

	{
		divil::ParticleBurst burst;
		burst.color       = glm::vec4(1.0f, 0.1f, 0.4f, 1.0f);
		burst.colorDecay  = glm::vec4(0.5f, 0.05f, 0.1f, 1.0f);
		burst.minAngle    = divil::DegToRad(-30.0f);
		burst.maxAngle    = divil::DegToRad(-150.0f);
		burst.minDistance = 0.0f;
		burst.maxDistance = 0.0f;
		burst.minVelocity = 15.0f;
		burst.maxVelocity = 250.0f;
		burst.minCount    = 15;
		burst.maxCount    = 25;
		burst.minDrag     = 0.2f;
		burst.maxDrag     = 0.2f;
		burst.minLifetime = 0.5f;
		burst.maxLifetime = 2.0f;
		mParticleSystem->AddBurst(mTransform.mPosition, burst);
	}

	Player::GetInstance()->AddMoney(mDeathValue);
	EnemyWaveSystem::RemoveItem(mHandle);
	EntitySystem::RemoveEntity(this);
}

void Enemy::NearestBuilding() {

	//if enemy is valid and does not have the ignore flag return
	Entity* e = EntitySystem::GetEntity(mAttackingBuilding);
	if (e)
	{
		if (!(e->mEntityFlags & EntityFlag_Ignore))
			return;
	}
	//else search for a target
		float closestBuilding = INFINITY;
		std::vector<Building*> temp = Player::GetInstance()->GetCurrentBuildings();
		for (int i = 0; i <temp.size(); i++)
		{
			Building* building = temp[i];
			if (!(building->mEntityFlags & EntityFlag_Ignore))
			{
				float distance = glm::distance(GetPosition(), building->GetPosition());
				if ((distance) <= closestBuilding)
				{
					mAttackingBuilding = building->mHandle;
					closestBuilding = distance;
				}
			}
		}
}
void Enemy::OnCollision(Entity* aEntity)
{
	Attack(aEntity);
}
glm::vec2 Enemy::RotateDirection(glm::vec2 aDirection, float aAngle) {
	glm::vec2 oldDirection = aDirection;
	aDirection.x = oldDirection.x * cos(aAngle) - oldDirection.y * sin(aAngle);
	aDirection.y = oldDirection.x * sin(aAngle) + oldDirection.y * cos(aAngle);
	oldDirection = glm::normalize(oldDirection);
	return aDirection;
}
bool Enemy::GetAttackFlag()
{
	return mAttacking;
}
void Enemy::MoveTo(float aDeltaTime,glm::vec2 aBuildingPos) {
	mJumpTimer -= aDeltaTime;
	
	if (mJumpTimer<=0.0f)
	{
		mTransform.mMoveDirection = aBuildingPos - GetPosition();
		mTransform.mMoveDirection = glm::normalize(mTransform.mMoveDirection);
		mJumpTimer = 0.5f;
		if (mJumpLeft) {
			mMoveOffset = 0.34906585;
			mJumpLeft = false;
		}
		else {
			mMoveOffset = 5.93411946;
			mJumpLeft = true;
		}
		mTransform.mMoveDirection = RotateDirection(mTransform.mMoveDirection, mMoveOffset);

		mTransform.mMoveDirection = glm::normalize(mTransform.mMoveDirection);
		mTransform.LookAt(mTransform.mMoveDirection);
	}
	mTransform.Move(aDeltaTime);
	
	//mTransform.mPosition = GetPosition() + (mMove) * mSpeed * aDeltaTime;
	//mTransform.mRotation = atan2( - aMoveDirection.x,aMoveDirection.y) *180.f / 3.14f;

}

void Enemy::DoDamage( int aDamageValue ) {
	mHealth -= aDamageValue;
	if (mHealth <= 0)
		OnDeath();
}
void Enemy::DebugMenu()
{
	ImGui::Text("Health : %d", GetHealth());
}
void Enemy::Attack(Entity* aEntity) 
{
	int temp = aEntity->GetKind();
	if (temp == Kind_Core || temp == Kind_Tower || temp == Kind_Turret)
	{
		Building* building = static_cast<Building*>(aEntity);
		if (building)
		{
			building->DoDamage(mAttackValue);
			mAttacking = true;
		}
	}
}
void Enemy::Update(float aDeltaTime) {

	NearestBuilding();

	Building* building = static_cast<Building*>(EntitySystem::GetEntity(mAttackingBuilding));
	if (building)
	{
		if (!mAttacking)
		{
			MoveTo(aDeltaTime, building->GetPosition());
		}
	}
	//TODO better way of defining if attacking has ended
	//we probably need an on end collision function for this
	mAttacking = false;
}

EnemyInspectable* getEnemyInspector()
{
	return EnemyInspectable::GetInstance();
}

divil::ParticleSystem *Enemy::mParticleSystem;
