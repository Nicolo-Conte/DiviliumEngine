#include "Entities/Grid.h"
#include "Entities/Transform.h"
#include "Rendering/DebugRenderer.h"

const float PI = 3.14159265;
#define RAD (180.0f / PI)

Transform::Transform() :
    mPosition(0, 0), mRotation(0), mScale(1, 1), mSize(10, 10), mUp(0, 1), mDirection(0, 0)
{  }

Transform::~Transform()
{  }

void Transform::Move(float aDeltaTime)
{
    if (mMoveDirection != glm::vec2(0.f, 0.f))
    {
        mPosition += (mMoveDirection * mMoveSpeed * aDeltaTime);
    }
    GetTilePositions();
}

void Transform::MoveTo(glm::vec2 aPosition, float aDeltaTime)
{
    mMoveDirection = aPosition - mPosition;
    float length = sqrt((mMoveDirection.x * mMoveDirection.x) + (mMoveDirection.y * mMoveDirection.y));
    mMoveDirection = mMoveDirection / length;

    mPosition = glm::vec2(mPosition + (mMoveDirection * mMoveSpeed * aDeltaTime));
    GetTilePositions();
}

void Transform::LookAt(Transform aTransform)
{
    mRotation = RotateToAngle(aTransform.mPosition) * RAD;
}

void Transform::LookAt(glm::vec2 aDirection)
{
    mRotation = atan2(aDirection.y * mUp.x - aDirection.x * mUp.y, aDirection.x * mUp.x + aDirection.y * mUp.y) * RAD;
}

float Transform::RotateToAngle(glm::vec2 aTarget)
{
    mDirection.x = mPosition.x - aTarget.x;
    mDirection.y = mPosition.y - aTarget.y;
    mDirection = glm::normalize(mDirection);

    return atan2(mDirection.y * mUp.x - mDirection.x * mUp.y, mDirection.x * mUp.x + mDirection.y * mUp.y);
}

// Slightly optimised by removing the divisions from this method
// Returns the tilepositions of the entity
void Transform::GetTilePositions()
{
    float rec = Grid::sInstance().mRec;
    glm::vec2 size = { mSize.x * mScale.x, mSize.y * mScale.y };

    glm::vec2 minXY = glm::vec2(floor(mPosition.x * rec), floor(mPosition.y * rec));
    glm::vec2 maxXY = glm::vec2(floor((mPosition.x + size.x) * rec), floor((mPosition.y + size.y) * rec));

    mTilePos.minXY = minXY;
    mTilePos.maxXY = maxXY;
}