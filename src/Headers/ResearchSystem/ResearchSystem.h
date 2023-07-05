#pragma once
#define MAXRESEARCH 5
#include "DebugUI/Inspectable.h"
namespace divil
{
	struct Widget;
	struct SpriteSheet;
}
enum ResearchLayer
{
  ResearchLayer_MainLayer,
  ResearchLayer_SubLayer,
  ResearchLayer_AditionalLayer,
};
class ResearchManager;
class ResearchNode;
class ResearchSystem:
	public Inspectable
{
	//measurements of imgui
	int mResearchLevel = 0;
	

public:
	int mMinX = -250;
	int mMinY = -150;
	int mOffsetX = 10;
	int mOffsetY = 20;
	int mSizeX = 250;
	int mSizeY = 100;

	int mResearchUnlocked = 0;

	void End();

	ResearchNode* mResearchTree[MAXRESEARCH][ResearchLayer_AditionalLayer + 1];

	divil::Widget * mWidget = nullptr;
	divil::SpriteSheet* mSheet = nullptr; 
	ResearchManager* mResearchManager;
	ResearchSystem();
	void Clear();
	//you can place a research note in a position from 0 to MAXRESEARCH -1
	//and there are three layers : MainLayer, SubLayer and AditionalLayer to define in which row you want to place it in
	int AddResearchNode(int aPosition, ResearchLayer aLayer, ResearchNode* node);
	//since there is no actual UI yet it will be handled trough imgui inspectables
	void Display() override;
	void Update();
	void DrawUpdate();
	void Open();
	void SetStates(uint32_t aPlayerClass, uint32_t aPlayerSubClass);
	void CoresComplete();

};