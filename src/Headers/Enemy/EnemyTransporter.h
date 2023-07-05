#pragma once
#include "glm/glm.hpp"
#include "player/player.h"
#include "Building/Building.h"

#include "entities/Entity.h"
#include <vector>
#include <algorithm>
#include "Enemy/Enemy.h"

class EnemyTransporter : public Enemy
{
public:
	EnemyTransporter();
	~EnemyTransporter();
	 
	void OnDeath() override;



private:

};

