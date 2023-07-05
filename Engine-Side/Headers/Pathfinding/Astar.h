#pragma once

class Astar
{

private:




	struct sNode
	{
		bool mObstacle = false;			// Is the node an obstruction?
		bool mVisited = false;			// Have we searched this node before?
		float mGlobalGoal;				// Distance to goal so far
		float mLocalGoal;				// Distance to goal if we took the alternative route
		float x;							// Nodes position in 2D space
		float y;
		std::vector<sNode*> mVecNeighbours;	// Connections to neighbours
		sNode* mParent;					// Node connecting to this node that offers shortest mParent
		~sNode(){}
	};

	static Astar* mAstar;
	sNode* mNode = nullptr;
	int mMapWidth = 64;		
	int mMapHeight = 36;	
	float mTileSize = 32.f;	
	int ScaleSize = 25;
	sNode* mNodeStart = nullptr;
	sNode* mNodeEnd = nullptr;
	std::vector<glm::vec2> p1;
	float Heuristic(sNode* aNode1, sNode* aNode2);
	
public:

	Astar() {}
	/**
   * Singletons should not be cloneable.
   */
	Astar(Astar const&) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(Astar const&) = delete;
	static Astar* GetInstance();
	~Astar() {}
	bool InitGrid(int aWidth, int aHeight, float aTileSize);
	void DrawAstar();
	std::vector<glm::vec2> SolveAStar(glm::vec2 aStartPosition, glm::vec2 aEndPosition);
	sNode* GetNode(glm::vec2 aPosition);
	void InsertObstacle(glm::vec2 aEntityPos);	//using world position
	void InsertObstacle(int x, int y);			//using index
	void RemoveObstacle(glm::vec2 aEntityPos);
	void SetTileSize(float aSize);
	void SetWidth(int aWidth);
	void SetHeight(int aHeight);
};