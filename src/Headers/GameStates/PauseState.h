#pragma once
#include "StateMachine\State.h"
class PauseState :
	public State
{

public:
	PauseState();
	void Init();
	void OnEnter() override;
	void OnEnd() override;
	bool Update(float aDt) override;
	void UnPauseGame();
	void BackToMainMenu();
};