#pragma once
#include "glm/glm.hpp"
#include "player/player.h"
#include "Building/Building.h"

#include "entities/Entity.h"
#include <vector>
#include <algorithm>
#include "Enemy/Enemy.h"

class EnemySniper : public Enemy
{
public:
	EnemySniper();
	~EnemySniper();

	void Attack(Entity* aEntity) override;
	void Update(float aDT);

private:
	float mAttackTimer;
	float mAttackDelay;

};

