#pragma once
#include "../Building/Building.h"
#include <vector>
#include "Entities/EntitySystem.h"
#include "../Enemy/Enemy.h"

//class WaveSystem;
class Turret :
    public Building
{

protected:
	

    int mAttackRange;
    int mAttackValue;
	bool mAttacking;
	bool mSellTurret = false;
    float mFireRate;
	float mAttackTimer;
	EntityHandle mEnemyTarget;
		


public:

	//Constructors & Destructor

	Turret();
	Turret(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
	~Turret();


	//Setter & Getter
	bool GetSellTurret();
	void SetSellTurret(bool aBool);
	
	/*
	int GetAttackRange();
	int GetAttackValue();
	float GetFireRate();
	bool GetAttacking();
	void SetAttackRange(int aAttackRange);
	void SetAttackValue(int aAttackValue);
	void SetFireRate(float aFireRate);
	void SetAttacking(bool aAttacking);
	*/


	//other functions

	virtual void AttackFunction();
	virtual void DeathFunction() override;
	virtual void Update(float aDeltaTime) override;
	virtual void DetectionRange();
	virtual void SellTurret();

	void DebugMenu() override;

};

inline bool Turret::GetSellTurret()
{
	return mSellTurret;
}

inline void Turret::SetSellTurret(bool aBool)
{
	mSellTurret = aBool;
}