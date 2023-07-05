#pragma once

#define MAX_GRID_TILES 256

struct EntityHandle;
class Entity;
class Grid
{
public:
    Grid() :
        mMinXY(0, 0), mMaxXY(0, 0), mTileSize(128.0f)
    {  }

    ~Grid()
    {  }

    static Grid& sInstance();
    void DrawGrid();
    void ClearGrid();

    glm::vec2 mMinXY;       // world size x
    glm::vec2 mMaxXY;       // world size y
    float mTileSize;    // Size of the cells
    const float mRec = 1.0f / mTileSize;
    const float mTileOffset = mTileSize * 0.5f;

    Entity* mGridMap[MAX_GRID_TILES][MAX_GRID_TILES];
    //divil::SmallMap<tilePos, divil::Array<Entity*>> mGridMap;
};
