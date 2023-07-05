#pragma once
#include "building/building.h"
class CoreManager;
class Core :
    public Building
{
	CoreManager* mCoreManager;

	float depletionTime = 10;
	float depletionTimer;

	float mDeathTime = 3;
	float MDeathTimer = -1;

	float mRadius = 50;

	bool mDeath = false;

	bool Overlapped();
public:

	//Constructors & Destructor

	Core(CoreManager* aCoreManager,glm::vec2 aPos);
	~Core();

	//other functions

	void DeathFunction();
	void Update(float aDeltaTime);
	void Draw(divil::RenderList* aRenderList);
	void DebugMenu();
};

