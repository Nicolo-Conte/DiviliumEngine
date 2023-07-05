#pragma once
#include <vector>
#include "glm/glm.hpp"

class Coordinates
{
public:
	int x;
	int y;
	glm::vec2 aPos; 
	Coordinates();
	Coordinates(int aX, int aY);

	int toFlat(int width);

	friend bool operator==(const Coordinates& c1, const Coordinates& c2);
	Coordinates& operator=(const Coordinates& other);
};


class Dijkstra
{
public:
	Dijkstra();
	~Dijkstra();
	/**
   * Singletons should not be cloneable.
   */
	Dijkstra(Dijkstra const&) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(Dijkstra const&) = delete;
	static Dijkstra* GetInstance();


	void Init(int aMapWidth, int aMapHeight, float aTileSize);
	void StartOnGrid(int aX, int aY);
	std::vector<glm::vec2> FindPath(glm::vec2 aStart, int aTargetValue);
	Coordinates GetCoordinate(glm::vec2 aPosition);
	void AddTargetName(char* aTargetName);
	void AddPathEnd(int aX, int aY);
	void AddPathObstacle(int aX, int aY);
	int MinDistance();
	void CreatPathingMap();
	Coordinates minDistance(std::vector<std::vector<int>> aDist, std::vector<std::vector<bool>> aExplored);
	void SetTileSize(float aSize);
	void SetWidth(int aWidth);
	void SetHeight(int aHeight);
	void InsertObstacle(glm::vec2 aEntityPos);
	void InsertObstacle(int x, int y) { mOcupationMap[x][y] = true; }
	void RemoveObstacle(glm::vec2 aEntityPos);

protected:
	int mMapWidth;
	int mMapHeight;
	float mTileSize;
	int mGridSize = 10;
	std::vector<std::vector<int>> mOcupationMap;
	std::vector<std::vector<int>> mDistance;
	std::vector<std::vector<bool>> mExplored;
	std::vector<std::vector<Coordinates>> mGrid;
	std::vector<std::vector<Coordinates>> mComeFrom;

	glm::vec2 mStart;

	std::vector<char*> mTargetsName;
	std::vector<glm::vec2> mPath;
	static Dijkstra* mDijkstra;


};

