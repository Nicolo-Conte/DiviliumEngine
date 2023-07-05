#include "building/tower.h"
#include "player/player.h"
#include "GameClass/GameClass.h"
#include "StateMachine/StateMachine.h"
#include <entities/EntitySystem.h>
#include <string>
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"
#include "GameStates/GameLoopState.h"
Tower::Tower()
{
	mCost = 0;
	mHealth = 100;
	mMaxHealth = mHealth;
	mName = "Tower";
	mKind = Kind_Tower;
	mTexture = "textures/Towers/Normal_0003_------------------------.png";
	mTransform.mSize = glm::vec2(50, 50);
}

Tower::~Tower()
{
}

/// <summary>
/// function called to the death of the Tower. This function will bring the game state to EndScreenState
/// </summary>
void Tower::DeathFunction()
{
	Player::GetInstance()->RemoveBuilding(this);
	EntitySystem::RemoveEntity(this);
	static_cast<GameLoopState*>(GameClass::GetInstance()->GetGameStates()->GetState("GameLoopState"))->EndGame();
}

/// <summary>
/// function that increases the player's money
/// </summary>
void Tower::GenerateMoney()
{
	Player::GetInstance()->AddMoney(1);
}

void Tower::Update(float) // Don't add the aDeltaTime if not using this is a better way to get rid of the warning from unused variable on ps4
{

	if (mHover)
	{
		GenerateMoney();
	}
}

void Tower::DebugMenu()
{
	ImGui::Text("health : %d",static_cast<int> (GetHealth()));
}

