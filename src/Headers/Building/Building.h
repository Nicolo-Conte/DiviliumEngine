#pragma once
#include "glm/glm.hpp"
#include "entities/Entity.h"
#include "DebugUI/Inspectable.h"
#include "Serialize/Serialize.h"
#include "../Building/BuildingValues.h"
#include <algorithm>
/// <summary>
/// represents the base class of the different types of constructions.
/// </summary>
class Building : public Entity
{
protected:


	int mCost;					//money cost of the building
	float mHealth;				//health value of the building
	//Sprite mSpriteBuilding;	//Sprite of the building
	float mRadiusSize = 10.f;
	bool mHover;
	int mHealingValue = 1;
	float mMaxHealth;

public:

	//Constructors & Destructor

	Building();
	~Building() { }


	//Setter & Getter

	int GetCost();
	float GetHealth();
	float GetMaxHealth();
	float GetRadiusSize();
	glm::vec2 GetPosition();
	bool GetHover();
	//Sprite GetSpriteBuilding();

	void SetCost(int aCost);
	void SetHealth(float aHealth);
	void SetPosition(glm::vec2 aPosition);
	void SetMaxHealth(float aHealth);
	void SetHover(bool aHover);
	//void SetSpriteBuilding(Sprite aSpriteBuilding);


	//other functions

	virtual void DeathFunction() = 0;				//function called to the death of the building
	void Draw(divil::RenderList* aRenderList) override { Entity::Draw(aRenderList); }
	void Update(float aDeltaTime) override = 0;
	void DoDamage(int aDamageValue);			//function to take damage from enemies
	void HealingFunction();

};

inline int Building::GetCost() {

	return mCost;
}

inline float Building::GetHealth()
{
	return mHealth;
}

inline float Building::GetMaxHealth()
{
	return mMaxHealth;
}

inline float Building::GetRadiusSize()
{
	return mRadiusSize;
 }


inline glm::vec2 Building::GetPosition() {

	return glm::vec2(mTransform.mPosition.x,mTransform.mPosition.y);
}

inline bool Building::GetHover()
{
	return mHover;
}

inline void Building::SetCost(int aCost)
{
	mCost = aCost;
}

inline void Building::SetHealth(float aHealth)
{
	mHealth = aHealth;
}

inline void Building::SetMaxHealth(float aMaxHealth)
{
	mMaxHealth = aMaxHealth;
}
inline void Building::SetPosition(glm::vec2 aPosition)
{
	mTransform.mPosition = aPosition;
	mTransform.GetTilePositions();
}

inline void Building::SetHover(bool aHover)
{
	mHover = aHover;
}


class BuildingInspectable : public Inspectable
{
	static BuildingInspectable* m_instance;
public:
	BuildingInspectable();

	static BuildingInspectable* GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new BuildingInspectable;
		}
		return m_instance;
	}

	void Display() override;
	

	float HEALTHMACHINEGUN = 1000.f;
	int COSTMACHINEGUN = 20;
	int ATTACKMACHINEGUN = 3;
	int RANGEMACHINEGUN = 500;
	float FIRERATEMACHINEGUN = 0.2f;

	float HEALTHSHOTGUN = 1500.f;
	int COSTSHOTGUN = 70;
	int ATTACKSHOTGUN = 2;
	int RANGESHOTGUN = 500;
	float FIRERATESHOTGUN = 0.05f;

	float HEALTHROCKET = 1500.f;
	int COSTROCKET = 90;
	int ATTACKROCKET = 12;
	int RANGEROCKET = 500;
	float FIRERATEROCKET = 2.f;
	float ROCKETEXPLOSIONRADIUS = 100.f;

private:
	void LoadSave(divil::SerializeMode aMode);
};
