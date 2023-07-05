#include "GameStates/PauseState.h"
#include "StateMachine/StateMachine.h"
PauseState::PauseState()
{
	mName = "PausedState";
}

void PauseState::Init()
{
}

void PauseState::OnEnter()
{
	printf("enter Pause state\n");
}

void PauseState::OnEnd()
{
	printf("enter Pause state\n");
}

bool PauseState::Update(float aDt)
{
	(void)aDt;
	return false;
}
void PauseState::UnPauseGame()
{
	mStateMachine->ChangeState("GameLoopState");
}

void PauseState::BackToMainMenu()
{
	mStateMachine->ChangeState("HomeScreenState");
}
