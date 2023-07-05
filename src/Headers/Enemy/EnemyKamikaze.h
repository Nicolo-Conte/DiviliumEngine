#pragma once
#include "glm/glm.hpp"
#include "player/player.h"
#include "Building/Building.h"

#include "entities/Entity.h"
#include <vector>
#include <algorithm>
#include "Enemy/Enemy.h"

class EnemyKamikaze : public Enemy
{
public:
	EnemyKamikaze();
	~EnemyKamikaze();
	
	void MoveTo(float aDeltaTime,glm::vec2 aBuildingPos) override;
	void Attack(Entity* aEntity) override;
	void DoDamage(int aDamageValue) override;

private:

};

