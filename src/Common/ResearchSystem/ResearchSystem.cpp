#include "ResearchSystem/ResearchSystem.h"
#include "Logger/Log.h"
#include "ResearchSystem/ResearchNode.h"
#include "Input/Input.h"
#include "glm/glm.hpp"
#include "Enemy/EnemyWaveSystem.h"
#include "GameClass/GameClass.h"
#include "Player/Player.h"
#include "Building/Tower.h"
#include "Divilium.h"
#include "Player/CoreManager.h"
#include "GameClass/GameClass.h"
#include "ResearchSystem/ResearchManager.h"
#include "Building/Mine.h"
#include "Entities/EntitySystem.h"
#include "Resources/ResourceManager.h"
#include "rendering/SpriteSheet.h"
#include "UI/Widget.h"
#include "Input/Input.h"
#include "rendering/Camera2D.h"
#include "Divilium.h"
#include "rendering/renderList.h"
ResearchSystem::ResearchSystem()
{
	mResearchManager = new ResearchManager();
	for (int y = 0; y <= ResearchLayer_AditionalLayer; y++)
	{
		for (int x = 0; x < MAXRESEARCH; x++)
		{
			mResearchTree[x][y] = nullptr;
		}
	}
	SetName("Temporary Imgui research tree");
	mHidden = true;
	mOverideBeginEnd = true;
}

int ResearchSystem::AddResearchNode(int aPosition, ResearchLayer aLayer, ResearchNode* aNode)
{
	
	if (aPosition >= MAXRESEARCH || aPosition < 0)
	{
		divil::LOG_MESSAGE(divil::LogLevel_Error, "research node had an invalid position");
		return 1;
	}
	if (mResearchTree[aPosition][aLayer])
	{
		divil::LOG_MESSAGE(divil::LogLevel_Warning, "research node position is already in use");
		return 2;
	}
	mResearchLevel++;
	mResearchTree[aPosition][aLayer] = aNode;
	return 0;
}


void ResearchSystem::Clear()
{
	mResearchLevel = 0;
	mResearchUnlocked = 0;
	mResearchManager->Reset();
	for (int y = 0; y <= ResearchLayer_AditionalLayer; y++)
	{
		for (int x = 0; x < MAXRESEARCH; x++)
		{
			if (mResearchTree[x][y])
			{
				ResearchNode* tempNode = mResearchTree[x][y];
				mResearchTree[x][y] = nullptr;
				delete tempNode;
			}
		}
	}
}

void ResearchSystem::Display()
{
	for (int x = 0; x <  MAXRESEARCH; x++)
	{
		for (int y = 0; y <= ResearchLayer_AditionalLayer; y++)
		{
			if (mResearchTree[x][y])
			{
				ImGui::SetNextWindowPos(ImVec2(static_cast<float>(50 + x *( mOffsetX + mSizeX)), static_cast<float>(50 + y * (mSizeY + mOffsetY))));
				ImGui::SetNextWindowSize(ImVec2(static_cast<float>(mSizeX), static_cast<float>(mSizeY)));
				std::string name = mResearchTree[x][y]->mName;
				name.append(" " + std::to_string(x) + " / " + std::to_string(y));
				ImGui::Begin(name.c_str(), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
				ImGui::Text(mResearchTree[x][y]->mDescription);
				if (!mResearchTree[x][y]->mActived && x <= mResearchUnlocked)
				{
					if (ImGui::Button("Activate"))
					{
						mResearchTree[x][y]->Execute();
						mResearchUnlocked++;
						End();
					}
				}
				ImGui::End();
			}
		}
	}
	
}

void ResearchSystem::Update()
{
	if (mOpen)
		mWidget->update();
}
void ResearchSystem::DrawUpdate()
{
#ifdef __ORBIS__
	if (mOpen)
		divil::GetRenderList()->DrawUI(mWidget);
#endif // __ORBIS__


}
void Execute(int x, int y)
{
	GameClass::GetInstance()->getResearchSystem()->mResearchTree[x][y]->Execute();
	GameClass::GetInstance()->getResearchSystem()->mResearchUnlocked++;
	GameClass::GetInstance()->getResearchSystem()->End();
}
void FindSpot()
{
	int mouseX, mouseY;
	divil::Input::GetMousePosition(&mouseX, &mouseY);

	glm::vec2 mouseP(mouseX, mouseY);
	mouseP = divil::GetCamera2D()->ToWorldSpace(mouseP);
	ResearchSystem* rs = GameClass::GetInstance()->getResearchSystem();
	mouseP -= glm::vec2(rs->mMinX,rs->mMinY);
	mouseP.x /= (rs->mOffsetX + rs->mSizeX);
	mouseP.y /= (rs->mOffsetY + rs->mSizeY);

	Execute((int)floor(mouseP.x), (int)floor(mouseP.y));
}

void ResearchSystem::Open()
{
	mOpen = true;
	divil::SetEntityVisibility(false);
	
	if (!mSheet)
	{
		mSheet = new divil::SpriteSheet(divil::ResourceManager::GetTexture("textures/text.png"), { 21, 21 });
	}
		if (mWidget)
			delete mWidget;

		mWidget = new divil::Widget(glm::vec2(mMinX, mMinY),glm::vec2(mSizeX,mSizeY));

		for (int x = 0; x < MAXRESEARCH; x++)
		{
			for (int y = 0; y <= ResearchLayer_AditionalLayer; y++)
			{
				if (mResearchTree[x][y])
				{
					glm::vec2 Pos(x * (mOffsetX + mSizeX), y * (mSizeY + mOffsetY));
				
					divil::Widget* w = mWidget->addWidget(glm::vec2(Pos), glm::vec2(mSizeX, mSizeY));
					w->addText(mResearchTree[x][y]->mName,mSheet,15.f);
					//mResearchTree[x][y]->mDescription
					//just text
					if (!mResearchTree[x][y]->mActived && x <= mResearchUnlocked)
					{
						//with button
						w->addButton(FindSpot);
					}
				
				}
			}
		}
}



void ResearchSystem::End()
{
	mOpen = false;
	//player -> new core wave
	EnemyWaveSystem::CreateWave();
	divil::SetEntityVisibility(true);
	Player::GetInstance()->GetCoreManager()->Start();
}
void ResearchSystem::CoresComplete()
{
	if (mResearchUnlocked < mResearchLevel)
	{
		EnemyWaveSystem::FinalWave();
	}
}

//this part is for setting the correct research slots for the skill tree
//based on the current classes



void IncreasePlayerHealth(float aValue)
{
	Player::GetInstance()->GetMainTower()->SetHealth(Player::GetInstance()->GetMainTower()->GetHealth() + aValue);
	Player::GetInstance()->GetMainTower()->SetMaxHealth(Player::GetInstance()->GetMainTower()->GetMaxHealth() + aValue);
}

void GiveMoney(float aValue)
{
	Player::GetInstance()->AddMoney(static_cast<int>(aValue));
}

void SpawnMines(Entity* aEntity)
{
	if (!aEntity)
		return;
	glm::vec2 pos = aEntity->mTransform.mPosition;
	EntitySystem::CreateEntity<Mine>(pos + glm::vec2(0,15));
	EntitySystem::CreateEntity<Mine>(pos + glm::vec2(0, -15));
	EntitySystem::CreateEntity<Mine>(pos + glm::vec2(15, 0));
	EntitySystem::CreateEntity<Mine>(pos + glm::vec2(-15, 0));
}

void ResearchSystem::SetStates(uint32_t aPlayerClass, uint32_t aPlayerSubClass)
{
	(void)aPlayerClass;
	//TOOO maybe a list similair to FFFLood
	/*
	switch (aPlayerClass)
	{
	case Bombardier:
	{	
		AddResearchNode(0, ResearchLayer_MainLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));

		break;
	}
	case Architect:
	{
		AddResearchNode(0, ResearchLayer_MainLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));

		break;
	}
	case Influencer:
	{
		AddResearchNode(0, ResearchLayer_MainLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));

		break;
	}
	case Economist:
	{
		AddResearchNode(0, ResearchLayer_MainLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));

		break;
	}
	default:
		divil::LOG_MESSAGE(divil::LogLevel_Warning, "skills cannot be set because of an invalid player main class");
		break;
	}
	switch (aPlayerSubClass)
	{
	case SpreadsheetSpecialist:
	{
		AddResearchNode(0, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));
		AddResearchNode(1, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 100", IncreasePlayerHealth, 100.f));
		AddResearchNode(2, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 150", IncreasePlayerHealth, 150.f));
		AddResearchNode(3, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 200", IncreasePlayerHealth, 200.f));
		AddResearchNode(4, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 250", IncreasePlayerHealth, 250.f));
		break;
	}
	case FieldTheoretician:
	{
		AddResearchNode(1, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 50$", GiveMoney, 50.f));
		AddResearchNode(2, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 100$", GiveMoney, 100.f));
		AddResearchNode(3, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 150$", GiveMoney, 150.f));
		AddResearchNode(4, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 200$", GiveMoney, 200.f));
		AddResearchNode(5, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 250$", GiveMoney, 250.f));
		break;
	}
	default:
		divil::LOG_MESSAGE(divil::LogLevel_Warning, "skills cannot be set because of an invalid player sub class");
		break;
	}
	*/
	switch (aPlayerSubClass)
	{
	case SpreadsheetSpecialist:
	{
		AddResearchNode(0, ResearchLayer_SubLayer, new ResearchNode("Increase health", "increase main tower health by 50", IncreasePlayerHealth, 50.f));
		break;
	}
	case FieldTheoretician:
	{
		AddResearchNode(0, ResearchLayer_SubLayer, new ResearchNode("get money", "Recieve a one time bonus of 50$", GiveMoney, 50.f));
		break;
	}
	}
	AddResearchNode(1, ResearchLayer_SubLayer, new ResearchNode("CoreDeplition", "cores deplete faster",-0.5f));
	AddResearchNode(2, ResearchLayer_SubLayer, new ResearchNode("CoreHealth", "increase core health", 50.f));

	AddResearchNode(3, ResearchLayer_SubLayer, new ResearchNode("SpawnCore", "Cores will spawn with mines", SpawnMines));
	

	AddResearchNode(0, ResearchLayer_MainLayer, new ResearchNode("AddRange", "Turrets have more range", 50.f, true));
	AddResearchNode(1, ResearchLayer_MainLayer, new ResearchNode("SpawnChance", "Enemies have 10% less chance to spawn", -10.f, true));
	AddResearchNode(2, ResearchLayer_MainLayer, new ResearchNode("PredictTime", "see the enemy spawn points earlier", 0.5f, true));
	//TODO probably want a switch also for the primary classes
}