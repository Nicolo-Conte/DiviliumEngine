#pragma once
#include "Input/Input.h"


class ResearchSystem;
class StateMachine;
class EntitySpawner;
class GameClass
{
	void InitializeGameStates();
	ResearchSystem* mResearchSystem = nullptr;
	StateMachine* mGameStates = nullptr;
	EntitySpawner* mEntitySpawner = nullptr;
public:

	//Input button
	divil::ButtonMappingID StartButton;
	divil::ButtonMappingID PlaceButton;
	divil::ButtonMappingID RightButton;
	divil::ButtonMappingID LeftButton;
	divil::ButtonMappingID AbilityButton;

	GameClass(GameClass& other) = delete;
	void operator=(const GameClass&) = delete;
	static GameClass* GetInstance();
	void Init();
	bool Update(float aDt);
	StateMachine* GetGameStates();
	ResearchSystem* getResearchSystem() { return mResearchSystem; }
	
	~GameClass();
protected:
	GameClass();
	static GameClass* mGameClass;
};

