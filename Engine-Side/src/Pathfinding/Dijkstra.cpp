#include "Pathfinding/Dijkstra.h"
#include "Rendering/DebugRenderer.h"
#include <include/Divilium.h>
#include <include/input/Input.h>
#include "Rendering/renderer.h"
#include "Rendering/Camera2D.h"
#include "entities/EntitySystem.h"


Dijkstra* Dijkstra::mDijkstra = nullptr;


Dijkstra* Dijkstra::GetInstance()
{
	if (mDijkstra == nullptr)
		mDijkstra = new Dijkstra();
	return mDijkstra;
}


Dijkstra::Dijkstra() {
	mGridSize = 0;
	mStart.x = 0.0f;
	mStart.y = 0.0f;
	
}
Dijkstra::~Dijkstra() {
}

void Dijkstra::Init(int aMapWidth, int aMapHeight, float aTileSize) {
	mMapWidth = aMapWidth;
	mMapHeight= aMapHeight;
	mTileSize = aTileSize;
	glm::vec2 minWorld;
	glm::vec2 maxWorld;
	divil::GetWorldMinMax(minWorld, maxWorld);
	mExplored.resize(mMapWidth, std::vector<bool>(mMapHeight));
	mDistance.resize(mMapWidth, std::vector<int>(mMapHeight));
	mOcupationMap.resize(mMapWidth, std::vector<int>(mMapHeight));
	mGrid.resize(mMapWidth, std::vector<Coordinates>(mMapHeight));
	mComeFrom.resize(mMapWidth, std::vector<Coordinates>(mMapHeight));
	for (int x = 0; x < mMapWidth; x++)
		for (int y = 0; y < mMapHeight; y++)
		{
			mExplored[x][y] = false;
			mDistance[x][y] = INT_MAX;
			mOcupationMap[x][y] = 0;
			mGrid[x][y].aPos.x = ((x + 1) * mTileSize) - (mTileSize/2) + minWorld.x;
			mGrid[x][y].aPos.y = ((y + 1) * mTileSize) - (mTileSize/2) + minWorld.y;
			mGrid[x][y].x = x;
			mGrid[x][y].y = y;
		}

}

std::vector<glm::vec2> Dijkstra::FindPath(glm::vec2 aStart, int aTargetValue) {

	mPath.clear();
	//mDistance[1][1] = 0; //start
	Coordinates start = GetCoordinate(aStart);
	mDistance[start.x][start.y] = 0;
	/*start.x = 1;
	start.y = 1;*/
	Coordinates comeFrom[10][10];
	mOcupationMap[6][6] = 2;
	std::vector<Coordinates> stepsToTarget;
	std::vector<Coordinates> allSteps;
	stepsToTarget;
	Coordinates current;

	Coordinates nextBest;
	Coordinates nextCoordinate;
	nextBest.x = 999;
	nextBest.y = 999;

	bool exploring = true;
	int count = 0;
	while (count < mMapWidth* mMapHeight)	//there was a 100 before, is it 10*10(the old width and height) or something else?
	{
		if (mOcupationMap[current.x][current.y] == aTargetValue)
		{
			break;
		}
		nextCoordinate = minDistance(mDistance, mExplored);
		//comeFrom[nextCoordinate.x][nextCoordinate.y] = current;
		current = nextCoordinate;
		allSteps.push_back(current);
		mExplored[current.x][current.y] = true;

		


		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				
				if (current.x + x >= 0 && current.x + x <= (mMapWidth-1) && current.y + y >= 0 && current.y + y <= (mMapHeight - 1))// need check so it wont look outside the grid
				{
					if (!mExplored[current.x + x][current.y + y]
						&& mDistance[current.x][current.y] != INT_MAX
						&& mOcupationMap[current.x][current.y] != 1 /*look if it is not wall or obstacle*/)
					{
						int speed = 1;
						if (x == 0 || y == 0)
						{
							speed = 1;

						}
						else {
							speed = 2;
						}
						if (mDistance[current.x + x][current.y + y] > mDistance[current.x][current.y] + speed)
						{
							mDistance[current.x + x][current.y + y] = mDistance[current.x][current.y] + speed;//add timeto get here from start Can add movement cost
							//nextCoordinate = minDistance(distance, explored);
							mComeFrom[current.x + x][current.y + y] = current;
						}

					}
				}
			}
		}

		


		count++;
	}
	
	Coordinates currentBack = current;
	while (exploring)
	{
		if (currentBack == start)
		{
			exploring = false;
		}
		else
		{
			stepsToTarget.push_back(currentBack);
			currentBack = mComeFrom[currentBack.x][currentBack.y];
		}
	}
	// can add path from comeFrom array
	
	reverse(stepsToTarget.begin(), stepsToTarget.end());
	std::vector<glm::vec2> path;
	for(size_t i = 0; i< stepsToTarget.size();i++)
		path.push_back(glm::vec2(stepsToTarget[i].x, stepsToTarget[i].y));


	return path;


}

Coordinates Dijkstra::GetCoordinate(glm::vec2 aPosition)
{
	int x = static_cast<int>((aPosition.x + (mTileSize / 2)) / mTileSize);
	int y = static_cast<int>((aPosition.y + (mTileSize / 2)) / mTileSize);
	return mGrid[x][y];
	//for (int x = 0; x < mMapWidth; x++)  
	//	for (int y = 0; y < mMapHeight; y++)
	//	{
	//		if (((aPosition.x >= mGrid[x][y].x - (mTileSize / 2)) && (aPosition.x < mGrid[x][y].x + (mTileSize / 2))) &&
	//			((aPosition.y >= mGrid[x][y].y - (mTileSize / 2)) && (aPosition.y < mGrid[x][y].y + (mTileSize / 2))))
	//		{
	//			return mGrid[x][y];
	//		}
	//	}
	//return Coordinates(0,0);
}


void Dijkstra::AddTargetName(char* aTargetName) {
	mTargetsName.push_back(aTargetName);
}

int Dijkstra::MinDistance() {

	return 0;
}
void Dijkstra::StartOnGrid(int aX, int aY) {
	mOcupationMap[aX][aY] = 1;
}

void Dijkstra::CreatPathingMap() {

}
void Dijkstra::AddPathEnd(int aX, int aY) {
	mOcupationMap[aX][aY] = 2;

}

void Dijkstra::AddPathObstacle(int aX, int aY) {
	mOcupationMap[aX][aY] = 3;
}

Coordinates::Coordinates()
{
	this->x = 0; // It is unlikely to have such values
	this->y = 0; // So this will be easier to spot
}

Coordinates::Coordinates(int aX, int aY)
{
	this->x = aX;
	this->y = aY;
}


int Coordinates::toFlat(int aWidth)
{
	return this->x * aWidth + this->y;
}

Coordinates& Coordinates::operator=(const Coordinates& aOther)
{
	/*if (*this == aOther)
	{
		return *this;
	}*/

	this->x = aOther.x;
	this->y = aOther.y;

	return *this;
}

bool operator==(const Coordinates& c1, const Coordinates& c2)
{
	return c1.x == c2.x && c1.y == c2.y;
}

Coordinates Dijkstra::minDistance(std::vector<std::vector<int>> aDist, std::vector<std::vector<bool>> aExplored)
{
	int min = INT_MAX;
	Coordinates minIndex = { INT_MAX, INT_MAX };

	for (int i = 0; i < mMapWidth; i++)
	{
		for (int j = 0; j < mMapHeight; j++)
		{
			if (aExplored[i][j] == false && aDist[i][j] <= min)
			{
				min = aDist[i][j];
				minIndex.x = i;
				minIndex.y = j;
			}
		}
	}

	return minIndex;
}

void Dijkstra::SetTileSize(float aSize)
{
	mTileSize = aSize;
}


void Dijkstra::SetWidth(int aWidth)
{
	mMapWidth = aWidth;
}

void Dijkstra::SetHeight(int aHeight)
{
	mMapHeight = aHeight;
}

void Dijkstra::InsertObstacle(glm::vec2 aEntityPos)
{
	mOcupationMap[GetCoordinate(aEntityPos).x][GetCoordinate(aEntityPos).y] = 1; // 1 == obstacle?
}

void Dijkstra::RemoveObstacle(glm::vec2 aEntityPos)
{
	mOcupationMap[GetCoordinate(aEntityPos).x][GetCoordinate(aEntityPos).y] = 0; // 1 == free place?
}