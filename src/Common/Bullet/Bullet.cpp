#include "src\RTSPrecomp.h"
#include "Bullet\Bullet.h"
#include "enemy/Enemy.h"
#include <entities/EntitySystem.h>
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"
Bullet::Bullet()
{
	mAttackRange = 5;
	mAttackValue = 0;
	mTransform.mMoveSpeed =	0;
	mFounddDirection = false;
	mLifeTimer = 1.0f;
	mName = "Bullet";
	mCollisionLayer = Collision::CreateCollisionLayerWithName("Bullets");
	mTexture = "textures/Ammo/towerDefense_tile273.png";
	mTransform.mSize = glm::vec2(10, 10);
}

Bullet::~Bullet()
{
}

void Bullet::DebugMenu()
{
	ImGui::Text("lifetime : %f", mLifeTimer);
}

void Bullet::Init(int aAttackValue, float , glm::vec2 aPosition, EntityHandle aTarget)
{
	mAttackValue = aAttackValue;
	mTransform.mMoveSpeed		 = 750.0f;
	mTransform.mPosition = aPosition;
	
	mTarget		 = aTarget;
	mTransform.mMoveDirection = glm::vec2(glm::vec2(static_cast<Enemy*>(EntitySystem::GetEntity(mTarget))->mTransform.mPosition - mTransform.mPosition));
	float length = sqrt((mTransform.mMoveDirection.x * mTransform.mMoveDirection.x) + (mTransform.mMoveDirection.y * mTransform.mMoveDirection.y));
	mTransform.mMoveDirection = mTransform.mMoveDirection / length;
	mTransform.GetTilePositions();
}

void Bullet::Init(int aAttackValue, float, glm::vec2 aPosition, EntityHandle aTarget, glm::vec2 aDirection)
{
	mAttackValue = aAttackValue;
	mTransform.mMoveSpeed = 750.0f;
	mTransform.mPosition = aPosition;
	
	mTransform.mMoveDirection = aDirection;
	mFounddDirection = true;
	mTarget = aTarget;
	mTransform.GetTilePositions();
}

void Bullet::Init(int aAttackValue, float, glm::vec2 aPosition)
{
	mAttackValue = aAttackValue;
	mTransform.mMoveSpeed = 750.0f;
	mTransform.mPosition = aPosition;
	mTransform.GetTilePositions();
}

void Bullet::DeathFunction()
{
	EntitySystem::RemoveEntity(this);
}

void Bullet::Update(float aDeltaTime)
{
	mLifeTimer -= aDeltaTime;
	if (!mFounddDirection)
	{
		PredictDirection(aDeltaTime);
		mFounddDirection = true;
	}
	else
	{
		mTransform.Move(aDeltaTime);
	}
	if (mLifeTimer <= 0.0f)
	{
		DeathFunction();
	}
}
void Bullet::OnCollision(Entity* aEntity)
{
	if (!aEntity)
		return;
	int temp = aEntity->GetKind();
	if (temp == Kind_Enemy || temp == Kind_EnemyKamikaze || temp == Kind_EnemySniper || temp == Kind_EnemyTransporter || temp == Kind_EnemyTank)
	{
		Attack(aEntity);
	}
}

void Bullet::Attack(Entity* aEntity)
{
	static_cast<Enemy*>(EntitySystem::GetEntity(aEntity->mHandle))->DoDamage(mAttackValue);
	DeathFunction();
}


bool Bullet::CheckDistance()
{
	Enemy* enemy = static_cast<Enemy*>(EntitySystem::GetEntity(mTarget));
	if (enemy)
	{
		float xd = enemy->GetPosition().x - GetPosition().x;
		float yd = enemy->GetPosition().y - GetPosition().y;
		float distance = sqrt(xd * xd + yd * yd);
		if (distance <= mAttackRange)
		{
			return true;
		}
	}
	return false;
}

void Bullet::PredictDirection(float)
{
	Enemy* enemy = static_cast<Enemy*>(EntitySystem::GetEntity(mTarget));
	if (enemy)
	{
		glm::vec2 vectorFromEnemy = mTransform.mPosition - enemy->mTransform.mPosition;
		float distanceToEnemy = CalculateLength(vectorFromEnemy);
		glm::vec2 EnemyVelocity = CalculateVelocity(enemy->GetDirection(), enemy->GetSpeed());
		float EnemySpeed = CalculateLength(EnemyVelocity);
		if(enemy->GetAttackFlag() == false)
		{
			float a = mTransform.mMoveSpeed * mTransform.mMoveSpeed - EnemySpeed * EnemySpeed;
			float b = 2 * ((vectorFromEnemy.x * EnemyVelocity.x) + (vectorFromEnemy.y * EnemyVelocity.y));
			float c = -distanceToEnemy * distanceToEnemy;

			float t1, t2;
			QuadraticSolver(a, b, c, t1, t2);

			if (t1 < 0 && t2 < 0)
			{
				// Both values for t are negative, so the interception would have to have
				// occured in the past
				return;
			}
			float m_timeToInterception = 0.0f;
			if (t1 > 0 && t2 > 0) // Both are positive, take the smaller one
				m_timeToInterception = fminf(t1, t2);
			else // One has to be negative, so take the larger one
				m_timeToInterception = fmaxf(t1, t2);

			glm::vec2 m_interceptionPosition = enemy->mTransform.mPosition + EnemyVelocity * m_timeToInterception;
			mTransform.mMoveDirection = glm::vec2((m_interceptionPosition - mTransform.mPosition) / m_timeToInterception);
			mTransform.mMoveDirection = mTransform.mMoveDirection / CalculateLength(mTransform.mMoveDirection);
			return;
		}
		else
		{
			glm::vec2 dir = enemy->mTransform.mPosition - mTransform.mPosition;
			mTransform.mMoveDirection = glm::vec3(dir / CalculateLength(dir), 0);
			return;
		}

	}
}

glm::vec2 Bullet::CalculateVelocity(glm::vec2 aDirection, float aSpeed)
{
	float d = sqrtf(aDirection.x * aDirection.x + aDirection.y * aDirection.y);
	return glm::vec2(aDirection.x / d * aSpeed, aDirection.y / d * aSpeed);

}

float Bullet::CalculateLength(glm::vec2 aVector)
{
	return sqrtf(aVector.x * aVector.x + aVector.y * aVector.y);
}

void Bullet::QuadraticSolver(float a, float b, float c, float& x1, float& x2)
{
	float d = (b * b) - (4 * a * c);
	if (d >= 0) {
		x1 = (-b + sqrt(d)) / (2 * a);
		x2 = (-b - sqrt(d)) / (2 * a);
	}
	else {
		d = d * (-1);
	}
	return;
}
