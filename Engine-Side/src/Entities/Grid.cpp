#include "Divilium.h"
#include "Rendering/DebugRenderer.h"
#include "Entities/Grid.h"
#include "Entities/Entity.h"

Grid& Grid::sInstance()
{
	static Grid instance;
	divil::GetWorldMinMax(instance.mMinXY, instance.mMaxXY);
	return instance;
}

// Draw the grid 2d array
void Grid::DrawGrid()
{
	DebugRenderer::sInstance().drawGrid(mMinXY.x + mTileOffset, mMinXY.y + mTileOffset, mMaxXY.x + mTileOffset, mMaxXY.y + mTileOffset, mTileSize, DebugColor::Magenta);
}

// Clear the grid 2d array
void Grid::ClearGrid()
{
	memset(mGridMap, 0, sizeof(mGridMap[0][0]) * MAX_GRID_TILES * MAX_GRID_TILES);

	//for (int i = 0; i < MAX_GRID_TILES; i++)
	//{
	//	for (int j = 0; j < MAX_GRID_TILES; j++)
	//	{
	//		mGridMap[i][j] = nullptr;
	//	}
	//}
}