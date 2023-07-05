#pragma once
#include "Tile.h"

struct Transform
{
    Transform();

    ~Transform();

    void Move(float aDeltaTime);
    void MoveTo(glm::vec2 aTarget, float aDeltaTime);
    void LookAt(Transform aTransform);
    void LookAt(glm::vec2 aDirection);
    void GetTilePositions();

    TilePositions mTilePos; // TilePosition
    glm::vec2 mPosition; // Position X, Y
    float mRotation;     // This is in radians i think >.<
    glm::vec2 mScale;    // Scale    X, Y
    glm::vec2 mSize;     // Size     X, Y

    float mMoveSpeed;
    glm::vec2 mUp;
    glm::vec2 mMoveDirection;
    glm::vec2 mDirection;

private:
    float RotateToAngle(glm::vec2 aTarget);

};
