#pragma once
#include "StateMachine/State.h"
namespace divil
{
	struct Widget;
	struct SpriteSheet;
}
class HomeScreenInspectable;
class HomeScreenState :
	public State
{

public:
	HomeScreenInspectable* mInspectable = nullptr;
	divil::Widget* mWidget;
	divil::SpriteSheet* mSheet;
	HomeScreenState();
	void Init();
	void OnEnter() override;
	void OnEnd() override;
	bool Update(float aDt) override;
	void Draw();
};
