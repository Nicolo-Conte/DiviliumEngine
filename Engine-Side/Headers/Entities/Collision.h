#pragma once

struct Collider
{
    Collider(glm::vec2 offset, float rad)
        : mOffset(offset), mRadius(rad)
    {  }

    Collider(float rad)
        : mOffset({0, 0}), mRadius(rad)
    {  }

    Collider(glm::vec2 pos, glm::vec2 offset, float rad, glm::vec2 size)
        : mPosition(pos), mOffset(offset), mRadius(rad), mSize(size), mScale({1,1})
    { }

    glm::vec2 mOffset;
    glm::vec2 mSize;
    glm::vec2 mScale;
    glm::vec2 mPosition;
    float mRadius;
};

class Entity;
//class divil::Serializer;
class Collision
{
public:
    Collision();
    ~Collision();

    static void Init();
    static bool EntityVsEntity(Entity* a, Entity* b);
    static bool CircleCollision(Entity* a, Entity* b);
    static bool PointVsEntity(glm::vec2 mousePos, Entity* b);

    // This is for unit testing purposes don't use it
    static bool CircleCollision(Collider a, Collider b);
    // This is for unit testing purposes don't use it
    static bool EntityVsEntity(Collider a, Collider b);

    static void SetCollisionLayerCondition(int aLayerA, int aLayerB, bool aCondition);
    static int CreateCollisionLayerWithName(const std::string &aName);
    static int GetCollisionLayerWithName(const std::string& aName);

    static bool CollisionLayer(int a, int b);
};
