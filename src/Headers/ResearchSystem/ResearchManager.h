#pragma once
#include "functional"
class Entity;
class ResearchManager
{
	std::unordered_map<const char*, float> mFloats;
	std::unordered_map<const char*, std::function<void(Entity*)>> mFunctions;
public:
	ResearchManager();
	//setting a variable based on a name
	//default variables should be set in the 
	//ResearchManager Reset function
	void SetVariable(const char* aName, float aFloat);
	//setting a functions based on a name
	//default functions should be set in the 
	//ResearchManager Reset function
	void SetFunction(const char* aName, std::function<void(Entity*)> mFunctions);
	//Getting a functions based on a name
	//on default it returns an empty function
	//if you want a default function then it should be set in the	
	//ResearchManager Reset function
	std::function<void(Entity*)> GetFunction(const char* aName);
	//Getting a variable based on a name
	//on default it returns 0.0f
	//if you want a default variable then it should be set in the	
	//ResearchManager Reset function
	float GetVariable(const char* aName);
	//resets the variables to its defaults
	void Reset();
};