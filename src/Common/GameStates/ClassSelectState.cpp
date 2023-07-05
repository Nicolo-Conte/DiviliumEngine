#include "GameStates/ClassSelectState.h"
#include "StateMachine/StateMachine.h"
#include "Building/Tower.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "Player/Player.h"
#include "Building/Turret.h"
#include "GameClass/GameClass.h"
#include "ResearchSystem/ResearchSystem.h"
#include "Player/CoreManager.h"
#include "rendering/SpriteSheet.h"
#include "UI/Widget.h"
#include "Resources/ResourceManager.h"
#include "Divilium.h"
#include "rendering/renderList.h"
class ClassSelectUI :
	public Inspectable
{
	ClassSelectState* mOwner;
	bool PlayerClass = false;
	bool SubClass = false;
public:
	ClassSelectUI(ClassSelectState* aOwner)
	{
		mOwner = aOwner;
		mHidden = true;
		mOpen = true;
		SetName("Select A class");
	}

	void Display() override
	{
		if (!PlayerClass)
		{
			ImGui::Text("Select Player Class");
			if (ImGui::Button("Bombardier"))
			{
				Player::GetInstance()->SetPlayerClass(mainPlayerClass::Bombardier);
				PlayerClass = true;
			}
			if (ImGui::Button("Architect"))
			{
				Player::GetInstance()->SetPlayerClass(mainPlayerClass::Architect);
				PlayerClass = true;
			}
			if (ImGui::Button("Influencer"))
			{
				Player::GetInstance()->SetPlayerClass(mainPlayerClass::Influencer);
				PlayerClass = true;
			}
			if (ImGui::Button("Economist"))
			{
				Player::GetInstance()->SetPlayerClass(mainPlayerClass::Economist);
				PlayerClass = true;
			}
			ImGui::Spacing();
		}
		else if (!SubClass)
		{
			ImGui::Text("Select Sub-Player Class");
			if (ImGui::Button("Spreadsheet Specialist"))
			{
				Player::GetInstance()->SetSubClass(subPlayerClass::SpreadsheetSpecialist);
				SubClass = true;
			}
			if (ImGui::Button("Field Theoretician"))
			{
				Player::GetInstance()->SetSubClass(subPlayerClass::FieldTheoretician);
				SubClass = true;
			}
		}
		else
		{
			mOwner->mStateMachine->ChangeState("GameLoopState");
		}
	}
};

void SelectBomb()
{
	Player::GetInstance()->SetPlayerClass(mainPlayerClass::Bombardier);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mPlayerClass = true;
}
void SelectArch()
{
	Player::GetInstance()->SetPlayerClass(mainPlayerClass::Architect);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mPlayerClass = true;
}
void SelectInf()
{
	Player::GetInstance()->SetPlayerClass(mainPlayerClass::Influencer);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mPlayerClass = true;
}
void SelectEco()
{
	Player::GetInstance()->SetPlayerClass(mainPlayerClass::Economist);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mPlayerClass = true;
}
void SelectSpread()
{
	Player::GetInstance()->SetSubClass(subPlayerClass::SpreadsheetSpecialist);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mSubClass = true;
}
void SelectField()
{
	Player::GetInstance()->SetSubClass(subPlayerClass::FieldTheoretician);
	static_cast<ClassSelectState*>(GameClass::GetInstance()->GetGameStates()->GetGurrentState())->mSubClass = true;
}

ClassSelectState::ClassSelectState()
{
	mName = "ClassSelectState";

}

void ClassSelectState::Init()
{
	mSheet = new divil::SpriteSheet(divil::ResourceManager::GetTexture("textures/text.png"), { 21, 21 });
	mWidget = nullptr;
}

void ClassSelectState::OnEnter()
{
	if(mWidget)
	delete mWidget;
	mWidget = new divil::Widget(glm::vec2(-50.f, 0.f), glm::vec2(300, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	mWidget->addText("select class", mSheet, 25);

	divil::Widget* bWidget = mWidget->addWidget(glm::vec2(0.f, 75.f), glm::vec2(250, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	bWidget->addText("bomber", mSheet, 25);
	bWidget->addButton(SelectBomb);

	divil::Widget* AWidget = mWidget->addWidget(glm::vec2(0.f, 150.f), glm::vec2(250, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	AWidget->addText("architect", mSheet, 25);
	AWidget->addButton(SelectArch);

	divil::Widget* EcoWidget = mWidget->addWidget(glm::vec2(0.f, 225.f), glm::vec2(250, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	EcoWidget->addText("economist", mSheet, 25);
	EcoWidget->addButton(SelectEco);

	divil::Widget* InfWidget = mWidget->addWidget(glm::vec2(0.f, 300.f), glm::vec2(250, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	InfWidget->addText("influencer", mSheet, 25);
	InfWidget->addButton(SelectInf);

	mPlayerClass = false;
	mSubClass = false;
	mSwitched = false;

	mUI = new ClassSelectUI(this);
}
void ClassSelectState::DisplaySubClasses()
{
	delete mWidget;

	mWidget = new divil::Widget(glm::vec2(0.f, -75.f), glm::vec2(450, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	mWidget->addText("select class",mSheet , 25);

	divil::Widget* bWidget = mWidget->addWidget(glm::vec2(0.f, 75.f), glm::vec2(450, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	bWidget->addText("spread sheet", mSheet, 25);
	bWidget->addButton(SelectSpread);

	divil::Widget* AWidget = mWidget->addWidget(glm::vec2(0.f, 150.f), glm::vec2(450, 50), 10, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
	AWidget->addText("Field Specialist", mSheet, 25);
	AWidget->addButton(SelectField);

}
void ClassSelectState::OnEnd()
{
	//spawn tower
	Player::GetInstance()->SpawnMainTower();

	//create the initial wave
	EnemyWaveSystem::CreateWave(0);
	EnemyWaveSystem::SpawnDistance(glm::vec2(150, 150), glm::vec2(200, 200));
	ResearchSystem* research = GameClass::GetInstance()->getResearchSystem();
	research->Clear();
	research->SetStates(Player::GetInstance()->GetPlayerClass(), Player::GetInstance()->GetSubClass());
	Player::GetInstance()->GetCoreManager()->Start();
	delete mUI;
	mUI = nullptr;
}

bool ClassSelectState::Update(float aDt)
{
	(void)aDt;
	if (mPlayerClass && !mSwitched)
	{
		DisplaySubClasses();
		mSwitched = true;
	}
	mWidget->update();
	if (mSubClass)
	{
		mStateMachine->ChangeState("GameLoopState");
	}
	return false;
}
void ClassSelectState::Draw()
{
#ifdef __ORBIS__
	divil::GetRenderList()->DrawUI(mWidget);
#endif // __ORBIS__

	
}
