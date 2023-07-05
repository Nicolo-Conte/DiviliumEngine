#include "Pathfinding/PathManager.h"
#include "Pathfinding/Astar.h"
#include "Pathfinding/Dijkstra.h"
#include "rendering/DebugRenderer.h"
#include "DebugUI/ImGuiHandler.h"
#include "BackGround/TileMapManager.h"
#include "BackGround/Tile.h"

#include "StateMachine/StateMachine.h"
#include "DebugUI/Inspectable.h"

PathManager* PathManager::mPathManager = nullptr;;
bool gActiveGridDraw = false;
bool gActivePathDraw = false;
bool gPathManagerInit = false;

class PathManagerInspector :
	public Inspectable
{
public:
	PathManagerInspector()
	{
		SetName("Path Manager");
	}
	void Display() override
	{
		//the gameplay specific options should only be shown while inside of the gameplay
		if (gPathManagerInit == true)
		{
		ImGui::Checkbox("draw the pathfinding grid", &gActiveGridDraw);
		ImGui::Checkbox("draw the last path calculated", &gActivePathDraw);
		}
	}
};

PathManager::PathManager()
{
	mPathManagerInspector = new PathManagerInspector();
}

PathManager* PathManager::GetInstance()
{
	if (mPathManager == nullptr)
		mPathManager = new PathManager();
	return mPathManager;
}

bool PathManager::InitGrid(int aWidth, int aHeight, float aTileSize)
{

	divil::TileMapManager::GetInstance()->GetTileSize(aTileSize, aTileSize);
	aWidth =  static_cast<int>(divil::TileMapManager::GetInstance()->GetMapXY().x);
	aHeight = static_cast<int>(divil::TileMapManager::GetInstance()->GetMapXY().y);
	Astar::GetInstance()->InitGrid(aWidth, aHeight, aTileSize);
	Dijkstra::GetInstance()->Init(aWidth, aHeight, aTileSize);

	for (int x = 0; x < aWidth; x++)
		for (int y = 0; y < aHeight; y++)
		{
			float obstacle = divil::TileMapManager::GetInstance()->GetTile(x, y)->GetSpeed();
			if (obstacle > 1)
			{
				InsertObstacle(x,y);
			}
		}
	gPathManagerInit = true;
	return true;
}

void PathManager::DrawGraph()
{
	//DebugRenderer::sInstance().drawGrid(
	//	0,
	//	0,
	//	(mMapHeight * mTileSize),
	//	(mMapWidth * mTileSize),
	//	mTileSize, 
	//	DebugColor::Black);
	Astar::GetInstance()->DrawAstar();
	return;
}


/// <summary>
/// Draw last path calculated
/// </summary>
void PathManager::DrawPath()
{
	if (mPath.size() > 1)
	{

		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			DebugRenderer::sInstance().drawLine((mPath[i].x), (mPath[i].y),
				(mPath[i + 1].x), (mPath[i + 1].y), DebugColor::Yellow);
		}
	}
}


void PathManager::SetTileSize(float aTileSize)
{
	mTileSize = aTileSize;
	Astar::GetInstance()->SetTileSize(aTileSize);
	Dijkstra::GetInstance()->SetTileSize(aTileSize);
}

void PathManager::SetGridSize(int aWidth, int aHeight)
{
	mMapWidth = aWidth;
	mMapHeight = aHeight;
	Astar::GetInstance()->SetWidth(aWidth);
	Astar::GetInstance()->SetHeight(aHeight);
	Dijkstra::GetInstance()->SetWidth(aWidth);
	Dijkstra::GetInstance()->SetHeight(aHeight);
}

void PathManager::UpdateGrid()
{

	if (gActiveGridDraw)
	{
		DrawGraph();
	}
	if (gActivePathDraw)
	{
		DrawPath();
	}
}


void PathManager::InsertObstacle(glm::vec2 aPosition)
{
	Astar::GetInstance()->InsertObstacle(aPosition);
	Dijkstra::GetInstance()->InsertObstacle(aPosition);
}
void PathManager::InsertObstacle(int x, int y)
{
	Astar::GetInstance()->InsertObstacle(x,y);
	Dijkstra::GetInstance()->InsertObstacle(x,y);
}
void PathManager::RemoveObstacle(glm::vec2 aPosition)
{
	Astar::GetInstance()->RemoveObstacle(aPosition);
	Dijkstra::GetInstance()->RemoveObstacle(aPosition);
}

std::vector<glm::vec2> PathManager::SearchPath(glm::vec2 aStartPosition, int aTargetValue )
{
	mPath = Dijkstra::GetInstance()->FindPath(aStartPosition, aTargetValue);
	return mPath;
}
std::vector<glm::vec2> PathManager::SearchPath(glm::vec2 aStartPosition, glm::vec2 aEndPosition)
{
	mPath = Astar::GetInstance()->SolveAStar(aStartPosition, aEndPosition);
	return mPath;
}

