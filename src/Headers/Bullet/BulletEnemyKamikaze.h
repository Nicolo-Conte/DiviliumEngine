#pragma once
#include "glm/glm.hpp"

#include "Bullet/Bullet.h"

class BulletEnemyKamikaze : public Bullet
{

public:

	BulletEnemyKamikaze();
	~BulletEnemyKamikaze();

	void Update(float aDeltaTime) override;

	//void OnCollision(Entity* aEntity) override;
	
	void Attack(Entity* aEntity) override;
	bool CheckDistance() override;
	void OnCollision(Entity* aEntity) override;
	void SetDirection(glm::vec3 aDirection) { mTransform.mMoveDirection = aDirection; }


private:
	
};

