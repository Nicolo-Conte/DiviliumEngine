#include "GameStates/HomeScreenState.h"
#include "StateMachine/StateMachine.h"
#include "Input/Input.h"
#include "GameClass/GameClass.h"
#include "BackGround/TileMapManager.h"
#include "rendering/SpriteSheet.h"
#include "UI/Widget.h"
#include "Resources/ResourceManager.h"
#include "Divilium.h"
#include "rendering/renderList.h"
#include "DebugUI/ImGuiHandler.h"
#include "DebugUI/Inspectable.h"

void StartGame()
{
	GameClass::GetInstance()->GetGameStates()->ChangeState("ClassSelectState");
}


class HomeScreenInspectable :
	public Inspectable
{
public:
	HomeScreenInspectable()
	{
		SetName("HomeScreen");
		mHidden = true;
		mOpen = true;
	}
	void Display() override
	{
		int x, y;
		divil::GetWindowSize(&x, &y);
		ImGui::SetNextWindowPos(ImVec2((float)(x/2),(float)(y/2)));
		if (ImGui::Button("Start Game"))
			StartGame();
	}


};




HomeScreenState::HomeScreenState()
{
	mName = "HomeScreenState";
}

void HomeScreenState::Init()
{
	mSheet = new divil::SpriteSheet(divil::ResourceManager::GetTexture("textures/text.png"), { 21, 21 });
	mInspectable = new HomeScreenInspectable();
	// test widget
	int x;
	int y;
	divil::GetWindowSize(&x,&y);
	glm::vec2 Size(250, 100);
	mWidget = new divil::Widget(glm::vec2(0, 0) - Size, Size, 10, glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
	mWidget->addText("start game", mSheet, 25);
	mWidget->addButton(StartGame);

	printf("enter home screen state\n");
}

void HomeScreenState::OnEnter()
{

}

void HomeScreenState::OnEnd()
{
	delete mInspectable;
	divil::TileMapManager::GetInstance()->RemoveMap();
	printf("leave home screen state\n");
}

bool HomeScreenState::Update(float aDt)
{
	mWidget->update();
	(void)aDt;
	if (divil::Input::ButtonPressed(GameClass::GetInstance()->StartButton))
		StartGame();
	return false;
}

void HomeScreenState::Draw()
{
#ifdef __ORBIS__
	divil::GetRenderList()->DrawUI(mWidget);
#endif // __ORBIS__


}