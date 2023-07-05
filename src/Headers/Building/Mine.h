#pragma once
#include "building/building.h"
class Mine :
    public Building
{
	float mDamage = 100;
public:

	Mine(glm::vec2 Pos);
	~Mine();

	void DeathFunction();
	void Update(float aDeltaTime);
	void DebugMenu();
	void OnCollision(Entity* aEntity);
};

