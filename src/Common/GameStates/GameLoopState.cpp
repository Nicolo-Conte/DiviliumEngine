#include "GameStates/GameLoopState.h"
#include "BackGround/TileMapManager.h"
#include "StateMachine/StateMachine.h"
#include "Pathfinding/PathManager.h"
#include "UI/Widget.h"
#include "Resources/ResourceManager.h"
#include "Divilium.h"
#include "rendering/renderList.h"
#include "Player/Player.h"
#include "rendering/Camera2D.h"
void GameLoopState::EndGame()
{
	mGameEnded = true;
}

GameLoopState::GameLoopState()
{
	mName = "GameLoopState";
}

void GameLoopState::Init()
{
	mSheet = new divil::SpriteSheet(divil::ResourceManager::GetTexture("textures/text.png"), { 21, 21 });
	mWidget = nullptr;
}

void GameLoopState::OnEnter()
{
	mGameEnded = false;
	if (!mMapGenerated)
	{
		mMapGenerated = true;
		divil::TileMapManager::GetInstance()->GenerateMap();
		PathManager::GetInstance()->InitGrid(64,36,32.f);
	}
	printf("enter game loop state\n");
}

void GameLoopState::OnEnd()
{
	printf("leave game loop state\n");
}

bool GameLoopState::Update(float aDt)
{
	(void)aDt;
	//game has to be ended at the start of the frame
	//and not during the collision update loop
	if (mGameEnded)
	{
		mMapGenerated = false;
		divil::TileMapManager::GetInstance()->RemoveMap();
		mStateMachine->ChangeState("EndScreenState");
	}
	if (mWidget)
		delete mWidget;

	int x, y;
	divil::GetWindowSize(&x, &y);
	glm::vec2 basePosition(0, y - 100);


	mWidget = new divil::Widget(divil::GetCamera2D()->ToWorldSpace(basePosition), glm::vec2(3000, 500), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	Player* player = Player::GetInstance();

	int aCost = player->GetAbilityCost();
	int bCost = player->GetBuildingCost(player->GetBuildingToBuy());
	int money = player->GetMoney();
	const char* building = player->GetBuildingName();
	mText.clear();
	mText = "current money ";
	mText.append(std::to_string(money));

	mText.append(" current building ");
	mText.append(building);
	mText.append(" building cost ");
	mText.append(std::to_string(bCost));
	mText.append(" ability cost ");
	mText.append(std::to_string(aCost));

	mWidget->addText(mText.c_str(), mSheet, 20);

	if(mWidget)
		mWidget->update();
	return true;
}
void GameLoopState::Draw()
{
#ifdef __ORBIS__
	if (mWidget)
		divil::GetRenderList()->DrawUI(mWidget);
#endif // __ORBIS__


}