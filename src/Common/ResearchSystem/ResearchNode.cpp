#include "ResearchSystem/ResearchNode.h"
#include "GameClass/GameClass.h"
#include "ResearchSystem/ResearchManager.h"
#include "ResearchSystem/ResearchSystem.h"
#include "Entities/Entity.h"
ResearchNode::ResearchNode(const char* aName, const char* aDescription, std::function<void(float)> aFunction,float aValue)
{
	mName = aName;
	mDescription = aDescription;
	mActivate = aFunction;
	mValue = aValue;
	type = 0;
}

ResearchNode::ResearchNode(const char* aName, const char* aDescription, std::function<void(Entity*)> aFunction)
{
	mName = aName;
	mDescription = aDescription;
	function = aFunction;
	type = 1;
}

ResearchNode::ResearchNode(const char* aName, const char* aDescription, float aValue, bool aAdition)
{
	type = 2 + aAdition;
	mName = aName;
	mDescription = aDescription;
	mValue = aValue;
}

void ResearchNode::Draw(float ,float )
{
}

void ResearchNode::Execute()
{
	switch (type)
	{
	case 0:
	{
		mActivate(mValue);
		break;
	}
	case 1:
	{
		GameClass::GetInstance()->getResearchSystem()->mResearchManager->SetFunction(mName, function);
		break;
	}
	case 2:
		GameClass::GetInstance()->getResearchSystem()->mResearchManager->SetVariable(mName, mValue);
		break;
	case 3:
	{
		ResearchManager* r = GameClass::GetInstance()->getResearchSystem()->mResearchManager;
		r->SetVariable(mName, r->GetVariable(mName) + mValue);
		break;
	}
	}
	mActived = true;
}
