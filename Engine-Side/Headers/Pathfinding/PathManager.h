#pragma once
class PathManagerInspector;

class PathManager
{
private:
	PathManagerInspector* mPathManagerInspector;
	static PathManager* mPathManager;
	std::vector<glm::vec2> mPath;
	int mMapWidth;		
	int mMapHeight;	
	float mTileSize;	

public:

	//Constructor & Deconstructor
	PathManager();
	/**
   * Singletons should not be cloneable.
   */
	PathManager(PathManager const&) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(PathManager const&) = delete;
	static PathManager* GetInstance();
	~PathManager() {}

	//Debug Functions
	void DrawGraph();
	void DrawPath();

	//Designer functions
	void MenageObstacles();	//designer tool that can use to place or remove obstacles.
	void SetGridSize(int aWidth, int aHeight);
	void SetTileSize(float aTileSize);

	//Entites Funcctions
	std::vector<glm::vec2> SearchPath(glm::vec2 aStartPosition, int aTargetValue);			//this function will call the Search function of Dijkstra
	std::vector<glm::vec2> SearchPath(glm::vec2 aStartPosition, glm::vec2 aEndPosition);	//this function will call the Search function of A*

	//Menage Grid
	bool InitGrid(int aWidth, int aHeight, float aTileSize);
	void UpdateGrid();	 
	void InsertObstacle(glm::vec2 aPosition );
	void InsertObstacle(int x, int y);
	void RemoveObstacle(glm::vec2 aPosition );
};