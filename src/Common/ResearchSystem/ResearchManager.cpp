#include "ResearchSystem/ResearchManager.h"
//an empty function used to fill up functions if they are unused
void Empty(Entity*) {}

ResearchManager::ResearchManager()
{	
}

void ResearchManager::SetVariable(const char* aName, float aFloat)
{
	auto temp = mFloats.find(aName);
	if (temp == mFloats.end())
		mFloats.insert(std::pair(aName, aFloat));
	else
		temp->second = aFloat;
}

void ResearchManager::SetFunction(const char* aName, std::function<void(Entity*)> aFunctions)
{
	auto temp = mFunctions.find(aName);
	if (temp == mFunctions.end())
		mFunctions.insert(std::pair(aName, aFunctions));
	else
		temp->second = aFunctions;
}

std::function<void(Entity*)> ResearchManager::GetFunction(const char* aName)
{
	
	auto temp = mFunctions.find(aName);
	if (temp == mFunctions.end())
		return Empty;
	else
		return temp->second;
}
//default return value is 0 if nothing is set
float ResearchManager::GetVariable(const char* aName)
{
	auto temp = mFloats.find(aName);
	if (temp == mFloats.end())
		return 0.0f;
	else
		return temp->second;
}

void ResearchManager::Reset()
{
	mFunctions.clear();
	mFloats.clear();
	//Enemy WaveSystem
	SetVariable("SpawnChance", 100.f);
	SetVariable("PredictTime", 1.f);
}


