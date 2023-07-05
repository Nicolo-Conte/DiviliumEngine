#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "../Building/Building.h"
#include <vector>
#include <string>
#include <time.h>
#include "Input/Input.h"

class Turret;
class Building;
class Tower;
class GameClass;
class PlayerInspector;
class Camera2D;
class Entity;
class CoreManager;

struct PlacePosition
{
	int x;
	int y;
};

enum mainPlayerClass {
		//---------------Player's classes--------------------------
		Bombardier = 0,
		Architect,
		Influencer,
		Economist,
	};

enum subPlayerClass {
	//---------------Player's sub classes--------------------------
	SpreadsheetSpecialist = 0,
	FieldTheoretician,
};

struct ExtraField
{
	ExtraField(glm::vec2 aPosition) : 
		mPosition(aPosition) {}
	glm::vec2 mPosition;
	float mTimer = 10.f;
	float mRadius = 10.f;
};

class Player
{
private:
	CoreManager* mCoreManager;
	//Constructors & Destructor
	Player();
	//the turret spawning can be disabled if you want to
	//prioritize someting else like EntitySpawning
	bool mMouseClickActive = true;

	int mMoney;										//amount of the player's money. It is used to purchase new buildings
	std::vector<Building*> mCurrentBuildings;		//in case the player needs to have the current buildings in play
	std::vector<Turret*> mCurrentTurrets;
	std::vector<Turret*> mTurretToTeleport;
	std::vector<ExtraField*> mExtraFields;
	glm::vec2 mStartTeleportPos;
	bool mTeleportFlag = false;
	int mBuildingToBuy;								//current building in the buildings selection to buy. This number must be in a range of [0 - 4]
	float mCursorRadius;							//the size of the radius around the cursor.
	static Player* mPlayer;
	Tower* mMainTower;
	PlacePosition mPlacePosition;
	PlayerInspector* mPlayerInspector;
	mainPlayerClass mPlayerClass;
	subPlayerClass mSubPlayerClass;
    

public:
	

	/**
   * Singletons should not be cloneable.
   */
	Player(Player const&)		  = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(Player const&) = delete;
	static Player* GetInstance();
	~Player();
	

	//Setter & Getter

	int GetMoney();
	int GetBuildingToBuy();
	std::vector<Building*> GetCurrentBuildings();
	Tower* GetMainTower();
	mainPlayerClass GetPlayerClass();
	subPlayerClass GetSubClass();

	void SetMoney(int aMoney);
	void SetBuildingToBuy(int aBuildingToBuy);
	void SetCurrentBuildings(std::vector<Building*> aBuildings);
	void SetMainTower(Tower* aMainTower);
	void SetPlayerClass(mainPlayerClass aPlayerClass);
	void SetSubClass(subPlayerClass aSubClass);
	//you can disable the left mouse click
	void SetMouseClickActive(bool aActive) { mMouseClickActive = aActive; } 
	bool GetMouseClickActive() { return mMouseClickActive; }
	CoreManager* GetCoreManager();
	void SetCoreManager(CoreManager* aCoreManager) { mCoreManager = aCoreManager; }

	//clear all of the buildings
	void Clear();
	void RemoveBuilding(Building* aBuilding);

	//other functions
	void SpawnMainTower();
	void AddMoney(int aAmount);
	void AddBuilding(Building* aBuilding);
	void AddTurret(Turret* aTurret);
	bool CanBuy(int aCost);
	void Update(float aDeltaTime);
	int GetBuildingCost(int aBuildingToBuy);
	int GetBuildingCost(std::string aName);
	int GetAbilityCost();
	char* GetBuildingName();
	bool AssignTurretPosition(glm::vec2 aPosition, float aRadius);
	bool Circle(float x1, float y1, float x2, float y2, float r1, float r2);


//------------------------------Player Controls------------------------------

	void CheckPlace(glm::vec2 aPosition, Entity* aEntity);
	void PlaceBuilding(int aBuilding,glm::vec2 aPosition);
	void CallAbility(glm::vec2 aPosition);
	void InputUpdate(float aDt);
	void HoverCheck(glm::vec2 aPosition);
	void CameraControl(float aDt);
	

//------------------------------Player Abilities------------------------------
	void BombAbility(glm::vec2 aPosition);
	void SellTurrets(glm::vec2 aPosition);
	void StartTeleportAbility(glm::vec2 aPosition);
	void EndTeleportAbility(glm::vec2 aPosition);
	void PlaceExtraField(glm::vec2 aPosition);
	void ExtraFieldsUpdate(float aDt);
};

inline CoreManager* Player::GetCoreManager()
{
	return mCoreManager;
}
inline mainPlayerClass Player::GetPlayerClass() {

	return mPlayerClass;
}

inline subPlayerClass Player::GetSubClass() {

	return mSubPlayerClass;
}


inline int Player::GetMoney() {

	return mMoney;
}

inline int Player::GetBuildingToBuy()
{
	return mBuildingToBuy;
}

inline std::vector<Building*> Player::GetCurrentBuildings()
{
	return mCurrentBuildings;
}

inline Tower* Player::GetMainTower()
{
	return mMainTower;
}

inline void Player::SetMoney(int aMoney)
{
	mMoney = aMoney;
}

inline void Player::SetBuildingToBuy(int aBuildingToBuy)
{
	mBuildingToBuy = aBuildingToBuy;
}

inline void Player::SetCurrentBuildings(std::vector<Building*>aBuildings)
{
	mCurrentBuildings = aBuildings;
}

inline void Player::SetMainTower(Tower* aMainTower)
{
	mMainTower = aMainTower;
}

inline void Player::SetPlayerClass(mainPlayerClass aMainClass) {

	mPlayerClass = aMainClass;
}

inline void Player::SetSubClass(subPlayerClass aSubClass) {

	mSubPlayerClass = aSubClass;
}