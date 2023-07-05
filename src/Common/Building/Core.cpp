#include "building/Core.h"
#include "player/player.h"
#include "GameClass/GameClass.h"
#include "StateMachine/StateMachine.h"
#include <entities/EntitySystem.h>
#include "rendering/renderList.h"
#include "Resources/ResourceManager.h"
#include "Player/CoreManager.h"
#include "ResearchSystem/ResearchSystem.h"
#include "ResearchSystem/ResearchManager.h"
Core::Core(CoreManager* aCoreManager, glm::vec2 aPos)
{
	mCoreManager = aCoreManager;
	ResearchManager* rManager = GameClass::GetInstance()->getResearchSystem()->mResearchManager;
	depletionTimer = depletionTime + rManager->GetVariable("CoreDeplition");
	mCost = 0;
	mHealth = 100 + rManager->GetVariable("CoreHealth");
	mMaxHealth = mHealth;
	mName = "Core";
	mKind = Kind_Core;
	mTransform.mPosition = aPos;
	mTransform.GetTilePositions();
	rManager->GetFunction("SpawnCore")(this);
	mTransform.mSize = glm::vec2(40, 40);
}

Core::~Core()
{
}

/// <summary>
/// function called to the death of the Tower. This function will bring the game state to EndScreenState
/// </summary>
void Core::DeathFunction()
{
	mDeath = true;
	MDeathTimer = mDeathTime;
	mEntityFlags |= EntityFlag_Ignore;
}

/// <summary>
/// function that increases the player's money
/// </summary>

void Core::Update(float aDT)
{
	if (mDeath)
	{
		MDeathTimer -= aDT;
		if (MDeathTimer < 0)
		{
			mHealth = mMaxHealth;
			mDeath = false;
			mEntityFlags &= ~EntityFlag_Ignore;
		}
		return;
	}
	else
	{
		if (mHover || Overlapped())
		{
			depletionTimer -= aDT;
			if (depletionTimer < 0)
			{
				//message the core manager
				mCoreManager->DeleteCore(this);
			}
		}
	}
}

void Core::Draw(divil::RenderList* aRenderList)
{
	if (mDeath)
	{

		aRenderList->DrawTexturedSprite(mTransform.mSize.x * mTransform.mScale.x, mTransform.mSize.y * mTransform.mScale.y,
			glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y),
			mTransform.mRotation, divil::ResourceManager::GetTexture("textures/Towers/Teleport_2_341x345.png"));
	}
	else
	{
		aRenderList->DrawTexturedSprite(mTransform.mSize.x * mTransform.mScale.x, mTransform.mSize.y * mTransform.mScale.y,
			glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y),
			mTransform.mRotation, divil::ResourceManager::GetTexture("textures/Towers/Teleporter_288x289.png"));
	}
}

void Core::DebugMenu()
{
	ImGui::Text("health : %d",static_cast<int> (GetHealth()));
	ImGui::Text("state : %s", mDeath ? "Death" : "Alive");
	ImGui::Text("Time left : %f", depletionTimer);
}

bool Core::Overlapped()
{
	float Rad2 = mRadius * mRadius;
	std::vector<Building*> temp = Player::GetInstance()->GetCurrentBuildings();
	for (int i = 0; i < temp.size(); i++)
	{
		if (!(temp[i]->GetKind() == Kind_Core))
		{
			glm::vec2 Pos = temp[i]->GetPosition();
			float x = mTransform.mPosition.x - Pos.x;
			float y = mTransform.mPosition.y - Pos.y;
			float distance = x * x + y * y;
			if (distance <= Rad2)
				return true;
		}
	}

	return false;
}
