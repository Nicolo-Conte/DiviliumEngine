#pragma once
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/Entity.h"
class EnemySpawner:
	public Entity
{
	int mMaxSpawn = 10;
	float mTime = 0.0;
	float mTimeInBetween = 0.05;
	Timing mTiming;
public:
	EnemySpawner(Timing aTiming, float xPos,float yPos);
	void Update(float aDeltaTime) override;

	bool Spawn();
};

