#pragma once
#include "glm/glm.hpp"
#include "vector"
class Enemy;
struct EntityHandle;


struct EnemyData
{
	uint32_t kind = 0;
	int amount =0;
};

struct Timing
{
	float Time = 0;
	std::vector<EnemyData> data;
};
struct ManualWave
{
	bool ImguiHeader = false;
	bool overide;
	int wave;
	EnemyData data;
	float Time;
	//wheter this function should overide
	//the current difficulty curve
	ManualWave(int aWave, bool aOveride) { wave = aWave; overide = aOveride; }
};

class EnemyWaveSystem
{
	EnemyWaveSystem() {}
	static void DefaultSpawn(int aWave);
	static bool OverideSpawn(int aWave);
public:
	static void Init();
	static int Update(float aDt);
	static bool WaveOver();
	static bool RemoveItem(EntityHandle aHandle);
	static int AddTimer(Timing aTime);
	static int SpawnEnemy(glm::vec2 aPos, Timing aTiming);
	static int CreateEnemySpawner(Timing aTiming);
	static int CreateWave(int aWave = -1);
	static Enemy* getEnemy(int aPlace);
	static int GetEnemyVecSize();
	static std::vector<EntityHandle>& GetEnemies();
	static void SpawnDistance(glm::vec2 aMinDistance,glm::vec2 aMaxDistance);
	static void SpawnAround(glm::vec2 aSpawnAround);
	static void Clear();
	static void AddEnemy(EntityHandle aHandle);
	static void SpawnEnemyRandom(EnemyData aData);
	static void SpawnEnemy(uint32_t aKind, glm::vec2 aPos);
	static void FinalWave();
	static void UpdateVariables();
};


