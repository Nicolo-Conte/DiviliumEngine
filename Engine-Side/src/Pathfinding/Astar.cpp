#include "Pathfinding\Astar.h"
#include "Rendering/DebugRenderer.h"
#include <include/Divilium.h>
#include <include/input/Input.h>
#include "Rendering/renderer.h"
#include "Rendering/Camera2D.h"
#include "entities/EntitySystem.h"

Astar* Astar::mAstar = nullptr;;


Astar* Astar::GetInstance()
{
	if (mAstar == nullptr)
		mAstar = new Astar();
	return mAstar;
}

bool Astar::InitGrid(int aWidth, int aHeight, float aTileSize)
{	
	glm::vec2 minWorld;
	glm::vec2 maxWorld;
	divil::GetWorldMinMax(minWorld, maxWorld);

	mMapWidth = aWidth ; 
	mMapHeight = aHeight ;
	mTileSize = aTileSize;
	
	float halfTile = mTileSize / 2;
	// Create a 2D array of mNode - this is for convenience of rendering and construction
		// and is not required for the algorithm to work - the mNode could be placed anywhere
		// in any space, in multiple dimensions...
	mNode = new sNode[mMapWidth * mMapHeight];
	for (int x = 0 ; x < mMapWidth ; x++)
		for (int y = 0; y < mMapHeight; y++)
		{
			mNode[y * mMapWidth + x].x = (( (x  ) + 1) * mTileSize) - halfTile; // ...because we give each node its own coordinates
			mNode[y * mMapWidth + x].y = (((y ) + 1) * mTileSize) - halfTile;
			mNode[y * mMapWidth + x].mObstacle = false;
			mNode[y * mMapWidth + x].mParent = nullptr;
			mNode[y * mMapWidth + x].mVisited = false;
		}

	// Create connections - in this case mNode are on a regular grid
	for (int x = 0; x < mMapWidth ; x++)
		for (int y = 0; y < mMapHeight ; y++)
		{
			if (y > 0)
				mNode[y * mMapWidth + x].mVecNeighbours.push_back(&mNode[(y - 1) * mMapWidth + (x + 0)]);
			if (y < mMapHeight - 1)
				mNode[y * mMapWidth + x].mVecNeighbours.push_back(&mNode[(y + 1) * mMapWidth + (x + 0)]);
			if (x > 0)
				mNode[y * mMapWidth + x].mVecNeighbours.push_back(&mNode[(y + 0) * mMapWidth + (x - 1)]);
			if (x < mMapWidth - 1)
				mNode[y * mMapWidth + x].mVecNeighbours.push_back(&mNode[(y + 0) * mMapWidth + (x + 1)]);

			// We can also connect diagonally
			if (y>0 && x>0)
				mNode[y*mMapWidth + x].mVecNeighbours.push_back(&mNode[(y - 1) * mMapWidth + (x - 1)]);
			if (y<mMapHeight-1 && x>0)
				mNode[y*mMapWidth + x].mVecNeighbours.push_back(&mNode[(y + 1) * mMapWidth + (x - 1)]);
			if (y>0 && x<mMapWidth-1)
				mNode[y*mMapWidth + x].mVecNeighbours.push_back(&mNode[(y - 1) * mMapWidth + (x + 1)]);
			if (y<mMapHeight - 1 && x<mMapWidth-1)
				mNode[y*mMapWidth + x].mVecNeighbours.push_back(&mNode[(y + 1) * mMapWidth + (x + 1)]);
			
		}

	//assign new position taking account of the min and max world
	for (int x = 0; x < mMapWidth; x++)
		for (int y = 0; y < mMapHeight ; y++)
		{
			mNode[y * mMapWidth + x].x = (mNode[y * mMapWidth + x].x - maxWorld.x); 
			mNode[y * mMapWidth + x].y = (mNode[y * mMapWidth + x].y - maxWorld.y);
		}
	// Manually positio the start and end markers so they are not nullptr
	mNodeStart = &mNode[0];
	mNodeEnd = &mNode[0 * mMapWidth + 1];
	return true;
}



/// <summary>
/// Update the grid mNode, inserting / removing obstacles from mNode
/// </summary>
void Astar::DrawAstar()
{
	DebugRenderer::sInstance().clear();
	/**/
	/*if (divil::Input::ButtonPressed(divil::AbstractControlIndex_T))
	{
		int x1, y1;
		divil::Input::GetMousePosition(&x1, &y1);
		int x2;
		int y2;
		divil::GetWindowSize(&x2, &y2);
		Camera2D* mCamera = new Camera2D(glm::vec2(0, 0), 0);
		Renderer::InitCamera(mCamera);
		glm::vec2 pos = glm::vec2(x1 + (-mCamera->pos.x * x2 / 2), y1 + (mCamera->pos.y * y2 / 2));

		for (int x = 0; x < mMapWidth; x++)
			for (int y = 0; y < mMapHeight; y++)
			{
				if (((pos.x >= mNode[y * mMapWidth + x].x - (mTileSize/2)) && (pos.x < mNode[y * mMapWidth + x].x + (mTileSize/2))) &&
					((pos.y >= mNode[y * mMapWidth + x].y - (mTileSize/2)) && (pos.y < mNode[y * mMapWidth + x].y + (mTileSize/2))))
				{
					mNode[y * mMapWidth + x].mObstacle = false;
				}
			}

	}
	if (divil::Input::ButtonPressed(divil::AbstractControlIndex_A))
	{
		int x1, y1;
		divil::Input::GetMousePosition(&x1, &y1);
		int x2;
		int y2;
		divil::GetWindowSize(&x2, &y2);
		Camera2D* mCamera = new Camera2D(glm::vec2(0, 0), 0);
		Renderer::InitCamera(mCamera);
		glm::vec2 pos = glm::vec2(x1 + (-mCamera->pos.x * x2 / 2), y1 + (mCamera->pos.y * y2 / 2));

		for (int x = 0; x < mMapWidth; x++)
			for (int y = 0; y < mMapHeight; y++)
			{
				if (((pos.x >= mNode[y * mMapWidth + x].x - (mTileSize/2)) && (pos.x < mNode[y * mMapWidth + x].x + (mTileSize / 2))) &&
					((pos.y >= mNode[y * mMapWidth + x].y - (mTileSize / 2)) && (pos.y < mNode[y * mMapWidth + x].y + (mTileSize / 2))))
				{
					mNode[y * mMapWidth + x].mObstacle = true;
					
				}
			}
	}*/

	DebugRenderer::sInstance().drawGrid(
		mNode[0].x,
		mNode[0].y,
		(mNode[(mMapHeight *mMapWidth ) -1]).x + mTileSize,
		(mNode[(mMapHeight * mMapWidth) - 1]).y + mTileSize,
		mTileSize, DebugColor::Black);

	/*for (int x = 0; x < mMapWidth; x++)
		for (int y = 0; y < mMapHeight; y++)
		{
			if (mNode[y * mMapWidth + x].mObstacle == true )
			{
				DebugRenderer::sInstance().drawCircle(mNode[y * mMapWidth + x].x,
					mNode[y * mMapWidth + x].y, (mTileSize / 2));
			}
		}*/

	//// Draw Path by starting ath the end, and following the mParent node trail
	/*if (p1.size()>1)
	{
		
		for (size_t i = 0; i<p1.size() - 1;i++)
		{
			DebugRenderer::sInstance().drawLine((p1[i].x ), (p1[i].y ),
				(p1[i + 1].x ), (p1[i+1].y ), DebugColor::Yellow);
		}
	}*/

	
	
	return;
}


std::vector<glm::vec2>  Astar::SolveAStar(glm::vec2 aStartPosition, glm::vec2 aEndPosition)
{
	mNodeStart = GetNode(aStartPosition);
	mNodeEnd = GetNode(aEndPosition);

	// Reset Navigation Graph - default all node states
	for (int x = 0; x < mMapWidth; x++)
		for (int y = 0; y < mMapHeight; y++)
		{
			mNode[y * mMapWidth + x].mVisited = false;
			mNode[y * mMapWidth + x].mGlobalGoal = INFINITY;
			mNode[y * mMapWidth + x].mLocalGoal = INFINITY;
			mNode[y * mMapWidth + x].mParent = nullptr;	// No mParents
		}

	// Setup starting conditions
	sNode* nodeCurrent = mNodeStart;
	mNodeStart->mLocalGoal = 0.0f;
	mNodeStart->mGlobalGoal = Heuristic(mNodeStart, mNodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered mNode get added to this
		// list, and will themselves be tested later
	std::list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(mNodeStart);

	while (!listNotTestedNodes.empty() && nodeCurrent != mNodeEnd)// Find absolutely shortest path // && nodeCurrent != mNodeEnd)
	{
		// Sort Untested mNode by global goal, so lowest is first
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->mGlobalGoal < rhs->mGlobalGoal; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain mNode that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->mVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid mNode left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->mVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->mVecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->mVisited && nodeNeighbour->mObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest mParent distance
			float fPossiblyLowerGoal = nodeCurrent->mLocalGoal + Heuristic(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->mLocalGoal)
			{
				nodeNeighbour->mParent = nodeCurrent;
				nodeNeighbour->mLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->mGlobalGoal = nodeNeighbour->mLocalGoal + Heuristic(nodeNeighbour, mNodeEnd);
			}
		}
	}

	std::vector<glm::vec2> QueuePathTarget;
	QueuePathTarget.push_back(glm::vec2(mNodeEnd->x, mNodeEnd->y));
	if (mNodeEnd != nullptr && mNodeEnd->mParent != nullptr)
	{

		sNode* p = mNodeEnd;
		while (p->mParent != nullptr)
		{
			QueuePathTarget.push_back(glm::vec2(p->mParent->x, p->mParent->y));

			// Set next node to this node's mParent
			p = p->mParent;
		}
		
	}

	//QueuePathTarget.
	std::reverse(QueuePathTarget.begin(), QueuePathTarget.end());
	p1 = QueuePathTarget;
	return QueuePathTarget;
}

float Astar::Heuristic(sNode* aNode1, sNode* aNode2)
{
	float x, y, x1, y1;
	x = static_cast<float>(aNode1->x);
	y = static_cast<float>(aNode1->y);
	x1 = static_cast<float>(aNode2->x);
	y1 = static_cast<float>(aNode2->y);

	return sqrtf((x - x1) * (x - x1) + (y - y1) * (y - y1));
}

void Astar::InsertObstacle(glm::vec2 aEntityPos)
{
	GetNode(aEntityPos)->mObstacle = true;
}

void Astar::InsertObstacle(int x, int y)
{
	mNode[y * mMapWidth + x].mObstacle = true;
}

void Astar::RemoveObstacle(glm::vec2 aEntityPos)
{
	GetNode(aEntityPos)->mObstacle = false;
}

void Astar::SetTileSize(float aSize)
{
	mTileSize = aSize;
}

void Astar::SetWidth(int aWidth)
{
	mMapWidth = aWidth;
}

void Astar::SetHeight(int aHeight)
{
	mMapHeight = aHeight;
}

Astar::sNode* Astar::GetNode(glm::vec2 aPosition)
{
	float halfTile = mTileSize / 2;
	for (int x = 0; x < mMapWidth; x++)
		for (int y = 0; y < mMapHeight; y++)
		{
			printf("x: %f, y:%f \n" ,mNode[y * mMapWidth + x].x, mNode[y * mMapWidth + x].y);
			if (((aPosition.x >= mNode[y * mMapWidth + x].x - halfTile) && (aPosition.x < mNode[y * mMapWidth + x].x + halfTile)) &&
				((aPosition.y >= mNode[y * mMapWidth + x].y - halfTile) && (aPosition.y < mNode[y * mMapWidth + x].y + halfTile)))
			{
				return &mNode[y * mMapWidth + x];
			}
		}
	return nullptr;
}
