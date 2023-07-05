#pragma once
#include "StateMachine\State.h"
namespace divil
{
	struct Widget;
	struct SpriteSheet;
}
class ClassSelectUI;
class ClassSelectState :
	public State
{
	ClassSelectUI* mUI = nullptr;
public:
	divil::Widget* mWidget;
	divil::SpriteSheet* mSheet;
	bool mSwitched = false;
	bool mPlayerClass = false;
	bool mSubClass = false;

	ClassSelectState();
	void Init();
	void OnEnter() override;
	void DisplaySubClasses();
	void OnEnd() override;
	bool Update(float aDt) override;
	void Draw();
};