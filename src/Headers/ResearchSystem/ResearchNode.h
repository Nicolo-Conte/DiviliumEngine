#pragma once
#include "functional"
class Entity;
class ResearchNode
{
	float mValue = 0;
	std::function<void(float)> mActivate;
	std::function<void(Entity*)> function;
	int type;
public:
	bool mActived = false;
	const char* mName;
	const char* mDescription;

	ResearchNode(const char* aName,const char* aDescription,std::function<void(float)> Function,float mValue = 0);
	ResearchNode(const char* aName, const char* aDescription, std::function<void(Entity*)> aFunction);
	//changes a variable inside of the research manager based on a variable
	ResearchNode(const char* aName, const char* aDescription, float aVariable,bool aAdition = false);
	void Draw(float xPos, float yPos);
	void Execute();
};