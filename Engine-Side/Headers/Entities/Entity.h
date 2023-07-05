#pragma once
#include "Entities/Transform.h"
#include "Entities/Collision.h"
#include "Entities/EntityHandle.h"

namespace divil
{
    class RenderList;
}


struct TilePositions;
class Entity
{
public:
    Entity() :
        mAlive(true), mName("Entity"), mHandle{ 0, 0 }, mKind(0), mEntityFlags(0), mCollider(10), mCollisionLayer(0)
    {
        mNextEntityInGrid = nullptr;
    }

    Entity(const std::string& name) :
        mAlive(true), mName(name), mHandle{ 0, 0 }, mKind(0), mEntityFlags(0), mCollider(10), mCollisionLayer(0)
    {
        mNextEntityInGrid = nullptr;
    }

    virtual ~Entity()
    {  }

    virtual void Draw(divil::RenderList* aRenderList);
    virtual void OnCollision(Entity* aEntity);
    virtual void Update(float aDeltaTime) = 0;
    virtual void DebugDraw(int aDrawShape = 1);
    virtual void DebugMenu()
    {  };

    bool mAlive;
    std::string mName; // NOTE(daniel): I changed this back to a std::string because const char * was not being used well, can be made something better later
    Collider mCollider;
    int mCollisionLayer;
    Transform mTransform;
    EntityHandle mHandle;
    uint32_t mEntityFlags;
    //texure to be drawn
    const char* mTexture = nullptr;

    Entity* mNextEntityInGrid = nullptr;

    uint32_t GetKind() const
    {
        return mKind;
    }

protected:
	// NOTE(daniel): user-defined kind enum, game programmers are responsible for maintaining
    // this enum and ensuring it gets set on the entity in its constructor.
    // For robustness this could be mandatory in the constructor for the base entity.
    uint32_t mKind;
};
