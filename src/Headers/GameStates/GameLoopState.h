#pragma once
#include "StateMachine/State.h"
namespace divil
{
	struct Widget;
	struct SpriteSheet;
}
class GameLoopState :
	public State
{
	std::string mText;
	bool mGameEnded = false;
	bool mMapGenerated = false;
public:
	divil::Widget* mWidget;
	divil::SpriteSheet* mSheet;
	void EndGame();
	GameLoopState();
	void Init();
	void OnEnter() override;
	void OnEnd() override;
	bool Update(float aDt) override;
	void Draw();
};

