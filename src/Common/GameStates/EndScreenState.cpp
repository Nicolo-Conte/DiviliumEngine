#include "GameStates/EndScreenState.h"
#include "StateMachine/StateMachine.h"
#include "Input/Input.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Player/Player.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "rendering/SpriteSheet.h"
#include "UI/Widget.h"
#include "Resources/ResourceManager.h"
#include "Divilium.h"
#include "rendering/renderList.h"
void BackToMainMenu()
{
	GameClass::GetInstance()->GetGameStates()->ChangeState("HomeScreenState");
}
EndScreenState::EndScreenState()
{
	mName = "EndScreenState";
}

void EndScreenState::Init()
{
	mSheet = new divil::SpriteSheet(divil::ResourceManager::GetTexture("textures/text.png"), { 21, 21 });
	mWidget = new divil::Widget(glm::vec2(0.f, 0.f), glm::vec2(250, 100), 10, glm::vec4(1.f, 0.f, 0.f, 1.f));
	mWidget->addText("return to main menu",mSheet,25);
	mWidget->addButton(BackToMainMenu);
}

void EndScreenState::OnEnter()
{
	EnemyWaveSystem::Clear();
	Player::GetInstance()->Clear();
	EntitySystem::ClearEntities();
	printf("enter end screen state\n");
}

void EndScreenState::OnEnd()
{
	printf("leave end screen state\n");
}

bool EndScreenState::Update(float aDt)
{
	(void)aDt;
	mWidget->update();
	if (divil::Input::ButtonPressed(GameClass::GetInstance()->StartButton))
		BackToMainGame();
	return false;
}
void EndScreenState::BackToMainGame()
{

	mStateMachine->ChangeState("HomeScreenState");
}

void EndScreenState::Draw()
{
#ifdef __ORBIS__
	divil::GetRenderList()->DrawUI(mWidget);
#endif // __ORBIS__


}