#pragma once
#include "../building/building.h"

class Player;
class GameClass;
class StateMachine;

class Tower :
    public Building
{
private:

public:

	//Constructors & Destructor

	Tower();
	~Tower();

	//other functions

	void DeathFunction();
	void GenerateMoney();
	void Update(float aDeltaTime);
	void DebugMenu();
};






