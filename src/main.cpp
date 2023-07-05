// ------------------------------------------------------------------
#include <time.h>
// ------------------------------------------------------------------
#include "Divilium.h"
#include "GameClass/GameClass.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/State.h"
#include "ResearchSystem/ResearchSystem.h"

/* Runs after engine init */
void GameInit(void)
{
    GameClass::GetInstance()->Init();
}

void GameUpdate(float dt)
{
    GameClass::GetInstance()->Update(dt);
    GameClass::GetInstance()->getResearchSystem()->Update();
}

void GameDrawUpdate()
{
    GameClass::GetInstance()->GetGameStates()->GetGurrentState()->Draw();
    GameClass::GetInstance()->getResearchSystem()->DrawUpdate();
}

void GameExit(void)
{

}

int main(int argc, char **argv)
{
    divil::DiviliumParams params;
	params.argc       = argc;
	params.argv       = argv;
	params.Init       = GameInit;
	params.Update     = GameUpdate;
	params.DrawUpdate = GameDrawUpdate;
	params.Exit       = GameExit;
    return divil::DiviliumInit(params);
}
