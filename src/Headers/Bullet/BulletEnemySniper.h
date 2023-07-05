#pragma once
#include "glm/glm.hpp"

#include "Bullet/Bullet.h"

class BulletEnemySniper : public Bullet
{

public:

	BulletEnemySniper();
	~BulletEnemySniper();

	void Update(float aDeltaTime) override;


	void Attack(Entity* aEntity) override;
	bool CheckDistance() override;
	
private:

};

