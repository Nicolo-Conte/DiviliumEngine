// ------------------------------------------------------------------

#include "Superluminal/PerformanceAPI.h"

// ------------------------------------------------------------------

#include "Entities/EntitySystem.h"
#include "Entities/Collision.h"
#include "Entities/Entity.h"
#include "Entities/Grid.h"
#include "DebugUI/Inspectable.h"
#include <unordered_set>

#define MAX_ENTITY_COUNT 16384
static size_t gEntityHandleCount = MAX_ENTITY_COUNT;
static EntityHandle gEntityHandles[MAX_ENTITY_COUNT];
static Entity* gEntities[MAX_ENTITY_COUNT];
static divil::Array<EntityHandle> mHandlesToRemove;

static bool gDrawGrid = false;
static int gTileOffset = static_cast<int>(Grid::sInstance().mTileOffset);
class EntitySystemInspector :
    public Inspectable
{
public:
    EntitySystemInspector()
    {
        SetName("EntitySystem");
    }
    void Display()
    {
        ImGui::Checkbox("Draw Debug Grid", &gDrawGrid);

        if(gDrawGrid)
        {
            //add grid debug draw
            Grid::sInstance().DrawGrid();
        }

        for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
        {
            if (gEntities[i] && gEntities[i]->mAlive)
            {
                std::string temp("Name : ");
                temp.append(gEntities[i]->mName);
                temp.append(" #" + std::to_string(i));

                if (ImGui::CollapsingHeader(temp.c_str()))
                {
                    ImGui::Text("Position : ");
                    ImGui::Text("%f", gEntities[i]->mTransform.mPosition.x);
                    ImGui::Text("%f", gEntities[i]->mTransform.mPosition.y);
                    ImGui::Spacing();
                    ImGui::Text("TilePosition : ");
                    ImGui::Text("min.x: %f & min.y: %f", gEntities[i]->mTransform.mTilePos.minXY.x, gEntities[i]->mTransform.mTilePos.minXY.y);
                    ImGui::Text("max.x: %f & max.y: %f", gEntities[i]->mTransform.mTilePos.maxXY.x, gEntities[i]->mTransform.mTilePos.maxXY.y);
                    ImGui::Spacing();
                    ImGui::Text("CollisionLayer : ");
                    ImGui::Text("%i", gEntities[i]->mCollisionLayer);
                    ImGui::Spacing();
                    ImGui::Text("Rotation : ");
                    ImGui::Text("%f", gEntities[i]->mTransform.mRotation);
                    ImGui::Spacing();

                    gEntities[i]->DebugMenu();
                }

            }

        }
    }
};
EntitySystemInspector* gEntitySystemInspector;

// Initialize the space for all of the entities allowed.
void EntitySystem::Init()
{
    Collision::Init();
    for (size_t i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        gEntityHandles[i].index = (uint32_t)(MAX_ENTITY_COUNT - i - 1);
    }
    gEntitySystemInspector = new EntitySystemInspector();
}

void EntitySystem::Exit()
{ }

// Adds entity to the total amount allowed
Entity* EntitySystem::AddEntity(Entity* aEntity)
{
    if (gEntityHandleCount == 0)
    {
        delete aEntity;
        return nullptr;
    }

    EntityHandle handle = gEntityHandles[--gEntityHandleCount];
    handle.generation += 1;

    Entity* e = gEntities[handle.index] = aEntity;
    e->mHandle = handle;
    e->mAlive = true;

    return e;
}

// Mark entity for removal
void EntitySystem::RemoveEntity(Entity* aEntity)
{
    aEntity->mAlive = false;
    mHandlesToRemove.Add(aEntity->mHandle);
}

// Remove all marked entities
void EntitySystem::RemoveEntities()
{
    for (int i = 0; i < mHandlesToRemove.Count(); ++i)
    {
        Entity* eToRemove = GetEntity(mHandlesToRemove[i]);
        if (eToRemove && !eToRemove->mAlive)
        {
            gEntityHandles[gEntityHandleCount++] = eToRemove->mHandle;
            gEntities[eToRemove->mHandle.index] = nullptr;
            delete eToRemove;
        }
    }

    mHandlesToRemove.Clear();
}

Entity* EntitySystem::GetEntity(const EntityHandle& aHandle)
{
    Entity* e = gEntities[aHandle.index];
    if (e && e->mHandle.generation == aHandle.generation)
    {
        return e; // Yes this is an entity
    }
    else
    {
        return nullptr; // no, it's someone else, in the same array slot
    }
}

void EntitySystem::UpdateEntities(float aDeltaTime)
{
    PERFORMANCEAPI_INSTRUMENT("Update Entities");
    Grid::sInstance().ClearGrid();
    for (int i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        Entity* entity = gEntities[i];
        if (entity && entity->mAlive)
        {
            // Update the entities
            entity->Update(aDeltaTime);

            // Make sure to check wheter it didn't get deleted during the update!
            //entity->mTransform.GetTilePositions();
            if (entity && entity->mAlive)
            {
                for (int x = static_cast<int>(entity->mTransform.mTilePos.minXY.x); x <= static_cast<int>(entity->mTransform.mTilePos.maxXY.x); x++)
                {
                    for (int y = static_cast<int>(entity->mTransform.mTilePos.minXY.y); y <= static_cast<int>(entity->mTransform.mTilePos.maxXY.y); y++)
                    {
                        entity->mNextEntityInGrid = Grid::sInstance().mGridMap[x + gTileOffset][y + gTileOffset];
                        Grid::sInstance().mGridMap[x + gTileOffset][y + gTileOffset] = entity;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        Entity* entity = gEntities[i];
        if (entity && entity->mAlive)
        {
            if (entity && entity->mAlive)
            {
                EntityCollission(entity);
            }
        }
    }
}

// Clear all entities on the screen
void EntitySystem::ClearEntities()
{
    for (int i = 0; i < MAX_ENTITY_COUNT; i++)
    {
        Entity* entity = gEntities[i];
        if (entity)
        {
            entity->mAlive = false;
            gEntityHandles[gEntityHandleCount++] = entity->mHandle;
            gEntities[entity->mHandle.index] = nullptr;
            delete entity;
        }
    }
}

// Returns current total entities
size_t EntitySystem::GetEntityCount()
{
    return (MAX_ENTITY_COUNT - gEntityHandleCount);
}

// Checks for collision between 2 entities
void EntitySystem::EntityCollission(Entity* aEntity)
{
    Entity* nextEntity = aEntity->mNextEntityInGrid;
    while (nextEntity)
    {
        if (aEntity != nextEntity)
        {
            //if (Collision::CollisionLayer(aEntity->mCollisionLayer, nextEntity->mCollisionLayer))
            //{
                if (Collision::CircleCollision(aEntity, nextEntity))
                {
                    aEntity->OnCollision(nextEntity);
                    //printf("Colliding!: %s, nextEntity: %s\n", aEntity->mName.c_str(), nextEntity->mName.c_str());
                }
            //}
        }

        nextEntity = nextEntity->mNextEntityInGrid;
    }
}

void EntitySystem::DrawEntities(divil::RenderList* aRenderList)
{
    for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
    {
        if (gEntities[i] && gEntities[i]->mAlive)
        {
            gEntities[i]->Draw(aRenderList);
        }
    }
}

// Debug Draw all entities
void EntitySystem::DebugDrawEntities()
{
    for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
    {
        if (gEntities[i] && gEntities[i]->mAlive)
        {
            gEntities[i]->DebugDraw();
        }
    }
}
