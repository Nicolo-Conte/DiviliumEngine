#pragma once
#include "glm/glm.hpp"
#include "../player/player.h"
#include "../Building/Building.h"

#include "entities/Entity.h"
#include <vector>
#include <algorithm>
#include <Serialize/Serialize.h>
#include "DebugUI/Inspectable.h"
#include "Particles/ParticleSystem.h"

class EnemyInspectable;
EnemyInspectable* getEnemyInspector();
static EnemyInspectable* mEnemyInspectable = nullptr;

class EnemyInspectable : public Inspectable
{
	EnemyInspectable()
	{
		SetName("Edit Enemies");
		SerializeEnemy(divil::SerializeMode::SerializeMode_Read);
	}
public:
	EnemyInspectable(EnemyInspectable const&) = delete;
	void operator=(EnemyInspectable const&) = delete;
	static EnemyInspectable* GetInstance()
	{
		if (mEnemyInspectable == nullptr)
			mEnemyInspectable = new EnemyInspectable();
		return mEnemyInspectable;
	}


	void Display() override
	{
		if (ImGui::CollapsingHeader("Enemy Kamikaze"))
		{
			ImGui::InputInt("Kamikaze Heath :", &mHealthEnemyKamikaze);
			ImGui::Text("amount of health : %d", mHealthEnemyKamikaze);
			ImGui::InputInt("Kamikaze Attack value :", &mAttackValueKamikaze);
			ImGui::Text("amount of Attack : %d", mAttackValueKamikaze);
			ImGui::InputFloat("Kamikaze Range value :", &mAttackRangeKamikaze);
			ImGui::Text("amount of Range : %f", mAttackRangeKamikaze);
			ImGui::InputFloat("Kamikaze Distance jump value:", &mDistanceJumpKamikaze);
			ImGui::Text("amount of Distance jump : %f", mDistanceJumpKamikaze);
			ImGui::InputFloat("Kamikaze Speed value :", &mSpeedKamikaze);
			ImGui::Text("amount of Speed : %f", mSpeedKamikaze);

			
		}
		if (ImGui::CollapsingHeader("Enemy Sniper"))
		{
			ImGui::InputInt("Sniper Heath :", &mHealthEnemySniper);
			ImGui::Text("amount of health : %d", mHealthEnemySniper);
			ImGui::InputInt("Sniper Attack value :", &mAttackValueSniper);
			ImGui::Text("amount of Attack : %d", mAttackValueSniper);
			ImGui::InputFloat("Sniper Range value :", &mAttackRangeSniper);
			ImGui::Text("amount of Range : %f", mAttackRangeSniper);
			ImGui::InputFloat("Sniper Distance jump value:", &mDistanceJumpSniper);
			ImGui::Text("amount of Distance jump : %f", mDistanceJumpSniper);
			ImGui::InputFloat("Sniper Speed value :", &mSpeedSniper);
			ImGui::Text("amount of Speed : %f", mSpeedSniper);
		}
		if (ImGui::CollapsingHeader("Enemy Soldier"))
		{
			ImGui::InputInt("Soldier Heath :", &mHealthEnemySoldier);
			ImGui::Text("amount of health : %d", mHealthEnemySoldier);
			ImGui::InputInt("Soldier Attack value :", &mAttackValueSoldier);
			ImGui::Text("amount of Attack : %d", mAttackValueSoldier);
			ImGui::InputFloat("Soldier Range value :", &mAttackRangeSoldier);
			ImGui::Text("amount of Range : %f", mAttackRangeSoldier);
			ImGui::InputFloat("Soldier Distance jump value:", &mDistanceJumpSoldier);
			ImGui::Text("amount of Distance jump : %f", mDistanceJumpSoldier);
			ImGui::InputFloat("Soldier Speed value :", &mSpeedSoldier);
			ImGui::Text("amount of Speed : %f", mSpeedSoldier);
		}
		if (ImGui::CollapsingHeader("Enemy Tank"))
		{
			ImGui::InputInt("Tank Heath :", &mHealthEnemyTank);
			ImGui::Text("amount of health : %d", mHealthEnemyTank);
			ImGui::InputInt("Tank Attack value :", &mAttackValueTank);
			ImGui::Text("amount of Attack : %d", mAttackValueTank);
			ImGui::InputFloat("Tank Range value :", &mAttackRangeTank);
			ImGui::Text("amount of Range : %f", mAttackRangeTank);
			ImGui::InputFloat("Tank Distance jump value:", &mDistanceJumpTank);
			ImGui::Text("amount of Distance jump : %f", mDistanceJumpTank);
			ImGui::InputFloat("Tank Speed value :", &mSpeedTank);
			ImGui::Text("amount of Speed : %f", mSpeedTank);
		}
		if (ImGui::CollapsingHeader("Enemy Transporter"))
		{
			ImGui::InputInt("Transporter Heath :", &mHealthEnemyTransporter);
			ImGui::Text("amount of health : %d", mHealthEnemyTransporter);
			ImGui::InputInt("Transporter Attack value :", &mAttackValueTransporter);
			ImGui::Text("amount of Attack : %d", mAttackValueTransporter);
			ImGui::InputFloat("Transporter Range value :", &mAttackRangeTransporter);
			ImGui::Text("amount of Range : %f", mAttackRangeTransporter);
			ImGui::InputFloat("Transporter Distance jump value:", &mDistanceJumpTransporter);
			ImGui::Text("amount of Distance jump : %f", mDistanceJumpTransporter);
			ImGui::InputFloat("Transporter Speed value :", &mSpeedTransporter);
			ImGui::Text("amount of Speed : %f", mSpeedTransporter);
		}
		if (ImGui::Button("Save"))
			Save();
	}

	void SerializeEnemy(divil::SerializeMode aMode)
	{
		divil::Serializer ser;
		if (BeginSerializeFile(ser, "Enemy.data", 0, aMode))
		{
			Serialize(ser, mHealthEnemyKamikaze);
			Serialize(ser, mAttackValueKamikaze);
			Serialize(ser, mAttackRangeKamikaze);
			Serialize(ser, mDistanceJumpKamikaze);
			Serialize(ser, mSpeedKamikaze);
			Serialize(ser, mHealthEnemySoldier);
			Serialize(ser, mAttackValueSoldier);
			Serialize(ser, mAttackRangeSoldier);
			Serialize(ser, mDistanceJumpSoldier);
			Serialize(ser, mSpeedSoldier);
			Serialize(ser, mHealthEnemySniper);
			Serialize(ser, mAttackValueSniper);
			Serialize(ser, mAttackRangeSniper);
			Serialize(ser, mDistanceJumpSniper);
			Serialize(ser, mSpeedSniper);
			Serialize(ser, mHealthEnemyTank);
			Serialize(ser, mAttackValueTank);
			Serialize(ser, mAttackRangeTank);
			Serialize(ser, mDistanceJumpTank);
			Serialize(ser, mSpeedTank);
			Serialize(ser, mHealthEnemyTransporter);
			Serialize(ser, mAttackValueTransporter);
			Serialize(ser, mAttackRangeTransporter);
			Serialize(ser, mDistanceJumpTransporter);
			Serialize(ser, mSpeedTransporter);
			EndSerializeFile(ser);
		}
	}
	void Save()
	{
		SerializeEnemy(divil::SerializeMode::SerializeMode_Write);
	}


	//Kamikaze variables
	int mHealthEnemyKamikaze = 10;
	int	  mAttackValueKamikaze = 10;
	float mDistanceJumpKamikaze = 5.0f;
	float mAttackRangeKamikaze = 5.f;
	float       mSpeedKamikaze = 150.f;

	//Soldier variables
	int   mHealthEnemySoldier = 10;
	int	  mAttackValueSoldier = 1;
   float mDistanceJumpSoldier = 5.0f;
	float mAttackRangeSoldier = 20.f;
	float       mSpeedSoldier = 100.0f;

	//Sniper variables
	int   mHealthEnemySniper = 10;
	int	  mAttackValueSniper = 1;
   float mDistanceJumpSniper = 5.0f;
	float mAttackRangeSniper = 20.f;
	float       mSpeedSniper = 100.0f;

	//Tank variables
	int   mHealthEnemyTank = 50;
	int	  mAttackValueTank = 1;
	float mDistanceJumpTank = 5.0f;
	float mAttackRangeTank = 5.f;
	float       mSpeedTank = 30.f;

	//Transporter variables
	int   mHealthEnemyTransporter = 20;
	int	  mAttackValueTransporter = 1;
	float mDistanceJumpTransporter = 5.0f;
	float mAttackRangeTransporter = 20.f;
	float       mSpeedTransporter = 50.f;
};

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	virtual void OnDeath();
	virtual void NearestBuilding();
	virtual void MoveTo(float aDeltaTime,glm::vec2 aBuildingPos);

	virtual void DoDamage(int aDamageValue);
	virtual void DebugMenu()override;
	virtual void Update(float aDeltaTime) override;
	virtual void Attack(Entity* aEntity);
	virtual int GetHealth() { return mHealth; }
	virtual void SetHealth(int aHealth) { mHealth = aHealth; };
	virtual void OnCollision(Entity* aEntity);
	virtual float GetSpeed() { return mTransform.mMoveSpeed; }
	virtual void SetSpeed(float aSpeed) { mTransform.mMoveSpeed = aSpeed; }

	virtual glm::vec2 GetPosition() { return mTransform.mPosition; }
	virtual void SetPosition(glm::vec2 aPosition) { mTransform.mPosition = aPosition; mTransform.GetTilePositions(); }

	virtual glm::vec2 GetDirection() { return mTransform.mMoveDirection; }

	virtual glm::vec2 RotateDirection(glm::vec2 aDirection, float aAngle);

	virtual bool GetAttackFlag();

protected:

	EntityHandle mAttackingBuilding;


	//glm::vec2 mDirecion;
	//glm::vec2 mMove;

	int mHealth;
	int mAttackValue;
	float mAttackRange;
	int mDeathValue;
	bool mMoving;
	bool mAttacking;
	float mDistanceJump;
	float mMoveOffset;
	float mJumpTimer;
	float mSpeed;
	bool mJumpLeft;
	EnemyInspectable* mInspect;

	static divil::ParticleSystem *mParticleSystem;
};

