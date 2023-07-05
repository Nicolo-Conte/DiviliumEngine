#pragma once
#include "Tile.h"
#include "Entities/EntityHandle.h"

namespace divil
{
class RenderList;

}

class Entity;
class EntitySystem
{
public:
    template <class E, typename ...Args>
    static inline E* CreateEntity(Args &&...args) { return static_cast<E*>(AddEntity(new E(static_cast<Args&&>(args)...))); }

    static void Init();
    static void Exit();
    static void RemoveEntity(Entity* aEntity);
    static void RemoveEntities();
    static Entity* GetEntity(const EntityHandle& aHandle);
    static void DebugDrawEntities();
    static void DrawEntities(divil::RenderList* aRenderList);
    static void UpdateEntities(float aDeltaTime);
    static void ClearEntities();
    static size_t GetEntityCount();

    static void EntityCollission(Entity* aEntity);

private:
    static Entity* AddEntity(Entity* aEntity);
};