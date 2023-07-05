#pragma once
#include "StateMachine/State.h"
namespace divil
{
	struct Widget;
	struct SpriteSheet;
}
class EndScreenState :
public	State
{

public:
	divil::Widget* mWidget;
	divil::SpriteSheet* mSheet;
	EndScreenState();
	void Init();
	void OnEnter() override;
	void OnEnd() override;
	bool Update(float DT) override;
	void BackToMainGame();
	void Draw();
};
