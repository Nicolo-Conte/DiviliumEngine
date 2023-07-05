#pragma once
#include "glm/glm.hpp"
#include "entities/Entity.h"
class Enemy;
class Bullet : public Entity
{
protected:

	int mAttackValue;
	float mAttackRange;
	EntityHandle mTarget;
	bool mFounddDirection;
	float mLifeTimer;

public:

	//Constructors & Destructor

	Bullet();
	~Bullet();


	//Setter & Getter

	virtual float GetSpeed() { return mTransform.mMoveSpeed; }
	virtual void SetSpeed(float aSpeed) { mTransform.mMoveSpeed = aSpeed; }

	virtual glm::vec2 GetPosition() { return mTransform.mPosition; }
	virtual void SetPosition(glm::vec2 aPosition) { mTransform.mPosition = aPosition; mTransform.GetTilePositions(); }

	//other functions
	void DebugMenu()override;
	virtual void Init(int aAttackValue, float aSpeed, glm::vec2 aPosition, EntityHandle aTarget);
	virtual void Init(int aAttackValue, float, glm::vec2 aPosition, EntityHandle aTarget, glm::vec2 aDirection);
	virtual void Init(int aAttackValue, float aSpeed, glm::vec2 aPosition);
	virtual void DeathFunction();
	virtual void Update(float aDeltaTime) override;
	virtual void OnCollision(Entity* aEntity);
	virtual void Attack(Entity* aEntity);
	virtual bool CheckDistance();
	virtual void PredictDirection(float aDeltaTime);
	virtual glm::vec2 CalculateVelocity(glm::vec2 aDirection, float aSpeed);
	float CalculateLength(glm::vec2 aVector);
	void QuadraticSolver(float a, float b, float c, float& x1, float& x2);
};

