#pragma once
#include "glm/glm.hpp"
class Core;
struct EntityHandle;
class CoreManager
{
	std::vector<EntityHandle> mCoresActive;
	int mTotalCores;
	int mMax;
	int mLeftToSpawn;
	glm::vec2 mMinDistance = glm::vec2(125,125);
	glm::vec2 mMaxDistance = glm::vec2(150, 150);
public:
	CoreManager();
	//resets the core
	void clear();
	//starts the next wave
	void Start();
	//spawn the cores
	void Spawn();
	void Place(glm::vec2 Pos);
	//delete a core
	void DeleteCore(Core* aCore);
};