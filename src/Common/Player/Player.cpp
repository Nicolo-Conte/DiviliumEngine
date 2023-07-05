#include "Divilium.h"
#include "input/Input.h"
#include "Player/Player.h"
#include "Player/CoreManager.h"
#include "Bullet/BulletRocket.h"

#include "Building/tower.h"
#include "Building/Rocket.h"
#include "Building/Sniper.h"
#include "Building/Minigun.h"
#include "Building/Shotgun.h"
#include "Building/Building.h"
#include "Building/Machinegun.h"

#include "DebugUI/ImGuiHandler.h"
#include "Enemy/EnemyWaveSystem.h"
#include "GameClass/GameClass.h"
#include "Rendering/renderer.h"
#include "Rendering/Camera2D.h"
#include "Rendering/DebugRenderer.h"
#include "StateMachine/StateMachine.h"
#include "ResearchSystem/ResearchSystem.h"

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(v,a,b) (MIN((b),MAX((v),(a))))

Player* Player::mPlayer = nullptr;
bool f1 = false;
bool gInfiniteMoney = false;

class PlayerInspector :
	public Inspectable
{
public:
	PlayerInspector()
	{
		SetName("controller");
	}
	void Display() override
	{
		//the gameplay specific options should only be shown while inside of the gameplay
		if (GameClass::GetInstance()->GetGameStates()->GetCurrentName() == "GameLoopState")
		{
			ImGui::Checkbox("infinite money", &gInfiniteMoney);
			ImGui::Text("amount of money : %d", Player::GetInstance()->GetMoney());
			ImGui::Text("Current Building : %s", Player::GetInstance()->GetBuildingName());

			ImGui::Spacing();
			if (ImGui::Button("fake cores depleted"))
			{
				GameClass::GetInstance()->getResearchSystem()->CoresComplete();
			}
		}
	}
};

Player::Player()
{
	mMoney = 100;	//set to 100 as in the ffflood game
	mBuildingToBuy = 0;
	mCursorRadius = 100.f;
	mPlayerInspector = new PlayerInspector();
	mPlayerClass = mainPlayerClass::Bombardier;
	mSubPlayerClass = subPlayerClass::SpreadsheetSpecialist;
	mCoreManager = new CoreManager();
}

void Player::Clear()
{
	mMoney = 100;
	mBuildingToBuy = 0;
	// But why do this
	// mCurrentBuildings.clear(); exists as well
	// iterator out of range error because mCurrentTurrets is already empty where is check for that ?
	for (auto& building : mCurrentBuildings)
	{
		EntitySystem::RemoveEntity(building);
	}
	mCurrentBuildings.clear();

	for (auto& turret : mCurrentTurrets)
	{
		EntitySystem::RemoveEntity(turret);
	}
	mCurrentTurrets.clear();

	mCoreManager->clear();
}

void Player::RemoveBuilding(Building* aBuilding)
{
	for(int i = 0; i < mCurrentBuildings.size();i++)
	{
		if(mCurrentBuildings[i] == aBuilding)
		{
			mCurrentBuildings.erase(mCurrentBuildings.begin() + i);
			return;
		}
	}
}

Player* Player::GetInstance()
{
	if (mPlayer == nullptr)
		mPlayer = new Player();
	return mPlayer;
}

void Player::SpawnMainTower()
{
	//get the screen size
	glm::vec2 Min;
	glm::vec2 Max;
	divil::GetWorldMinMax(Min, Max);
	glm::vec2 Centre = Min + ((Max - Min) * 0.5f);

	int randx = (int)Centre.x + rand() % 200 - 100;	//to change
	int randy = (int)Centre.y + rand() % 200 - 100;	//to change
	Entity* t = EntitySystem::CreateEntity<Tower>();
	t->mTransform.mPosition = glm::vec2(randx, randy);
	t->mTransform.GetTilePositions();
	AddBuilding(static_cast<Building*>(t));
	SetMainTower(static_cast<Tower*>(t));
	//make it so the enemies spawn around the tower
	EnemyWaveSystem::SpawnAround(glm::vec2(t->mTransform.mPosition.x, t->mTransform.mPosition.y));
	f1 = true;
}

void Player::AddMoney(int aAmount)
{
	mMoney += aAmount;
}

void Player::AddBuilding(Building* aBuilding)
{
	mCurrentBuildings.push_back(aBuilding);
}

void Player::AddTurret(Turret* aTurret)
{
	mCurrentTurrets.push_back(aTurret);
}

/// <summary>
/// function to be used to place a building on the playing field.
/// This function is called when the left mouse button is clicked, the location of the building corresponds to the position of the cursor on the screen
/// </summary>
/// <param name="aBuilding"></param>
void Player::PlaceBuilding(int aBuilding, glm::vec2 aPosition )
{
	auto* buildingInst = BuildingInspectable::GetInstance();
		Entity* entity;
		switch (aBuilding)	//check which type of turret the player have chosen, create the turret and place it
		{
			case 0:
			{
				//create Machinegun
				entity = EntitySystem::CreateEntity<Machinegun>(buildingInst->RANGEMACHINEGUN, buildingInst->ATTACKMACHINEGUN, buildingInst->FIRERATEMACHINEGUN, buildingInst->HEALTHMACHINEGUN);
				CheckPlace(aPosition,entity);
				break;
			}
			case 1:
			{
				//create Minigun
				entity = EntitySystem::CreateEntity<Minigun>(RANGEMINIGUN, ATTACKMINIGUN, FIRERATEMINIGUN, HEALTHMINIGUN);
				CheckPlace(aPosition, entity);
				break;
			}
			case 2:
			{
				//create Shotgun
				entity = EntitySystem::CreateEntity<Shotgun>(buildingInst->RANGESHOTGUN, buildingInst->ATTACKSHOTGUN, buildingInst->FIRERATESHOTGUN, buildingInst->HEALTHSHOTGUN);
				CheckPlace(aPosition, entity);
				break;
			}
			case 3:
			{
				//create Rocket
				entity = EntitySystem::CreateEntity<Rocket>(buildingInst->RANGEROCKET, buildingInst->ATTACKROCKET, buildingInst->FIRERATEROCKET, buildingInst->HEALTHROCKET);
				CheckPlace(aPosition, entity);
				break;
			}
			case 4:
			{
				//create Sniper
				entity = EntitySystem::CreateEntity<Sniper>(RANGESNIPER, ATTACKSNIPER, FIRERATESNIPER, HEALTHSNIPER);
				CheckPlace(aPosition, entity);
				break;
			}
			default:
			{
				printf("Something gone wrong");
				break;
			}
		}
}

void Player::InputUpdate(float aDt)
{
	divil::Input::GetMousePosition(&mPlacePosition.x, &mPlacePosition.y);
	glm::vec2 MousePos = divil::ConvertScreenPosToGameScreen(glm::vec2(mPlacePosition.x,mPlacePosition.y));
	int x, y;
	Camera2D* cam = divil::GetCamera2D();
	divil::GetWindowSize(&x, &y);
	glm::vec2 pos = glm::vec2(MousePos.x + (-cam->pos.x * x / 2), MousePos.y + (cam->pos.y * y / 2));

	//building placement
	if (mMouseClickActive)
	{
		if (divil::Input::ButtonPressed(GameClass::GetInstance()->PlaceButton) && !divil::ImGuiHandler::IsAWindowHovered())
		{
			if (CanBuy(GetBuildingCost(mBuildingToBuy)) == true) //Check if the player has enough money to buy the building
				PlaceBuilding(mBuildingToBuy, pos);
			else
				printf("the building can't be placed");
		}
	}

	//active ability
	if (divil::Input::ButtonPressed(GameClass::GetInstance()->AbilityButton) && !divil::ImGuiHandler::IsAWindowHovered())
	{
		CallAbility(pos);
	}

	//building selection
	if (divil::Input::ButtonPressed(GameClass::GetInstance()->LeftButton))
	{
		mBuildingToBuy--;
		if (mBuildingToBuy < 0)
			mBuildingToBuy = 4;
	}
	if (divil::Input::ButtonPressed(GameClass::GetInstance()->RightButton))
	{
		mBuildingToBuy++;
		if (mBuildingToBuy > 4)
			mBuildingToBuy = 0;
	}
	if (divil::Input::ButtonReleased(GameClass::GetInstance()->AbilityButton))
	{
		EndTeleportAbility(pos);
	}

	HoverCheck(glm::vec2(pos.x, pos.y));
	ExtraFieldsUpdate(aDt);

}

char* Player::GetBuildingName()
{
	switch (mBuildingToBuy)
	{
	case 0:
	{
		return NAMEMACHINEGUN;
	}
	case 1:
	{
		return NAMEMINIGUN;
	}
	case 2:
	{
		return NAMESHOTGUN;
	}
	case 3:
	{
		return NAMEROCKET;
	}
	case 4:
	{
		return NAMESNIPER;
	}
	default:
		printf("Something gone wrong");
		break;
	}
	return 0;
}

void Player::CameraControl(float aDt)
{
	(void)aDt;
	Camera2D* cam = divil::GetCamera2D();
	int iX, iY;
	divil::Input::GetMousePosition(&iX, &iY);
	glm::vec2 Mouse = divil::ConvertScreenPosToGameScreen(glm::vec2(iX, iY));
	int sX, sY;
	divil::GetWindowSize(&sX, &sY);
	
	float difX, difY;
	
	float offsetX = 1.f -((float)Mouse.x / (float)sX);
	if (offsetX > 1 || offsetX < 0)
		return;
	float offsetY = ((float)Mouse.y / (float)sY);
	if (offsetY > 1 || offsetY < 0)
		return;

	difX = (cam->mCameraOffset.MaxX - cam->mCameraOffset.minX);
	difY = cam->mCameraOffset.MaxY - cam->mCameraOffset.minY;

	float CamX = cam->mCameraOffset.minX + offsetX * difX;
	float CamY = cam->mCameraOffset.minY + offsetY * difY;
	//CamX += 1;
	//CamY += 1;
	cam->SetMoveToLocation(glm::vec2(CamX, CamY));
	
	/*
	if (divil::Input::ButtonPressed(divil::AbstractControlIndex_H))
	{
		cam->SetMoveToLocation(glm::vec2(cam->pos.x, cam->pos.y -= 0.1f));
	}
	if (divil::Input::ButtonPressed(divil::AbstractControlIndex_G))
	{
		cam->SetMoveToLocation(glm::vec2(cam->pos.x, cam->pos.y+= 0.1f));
	}
	*/
//	glm::vec2 wPos = cam->ToWorldSpace(glm::vec2(iX,iY));
//	printf("X: %f\n", wPos.x);
	/*
	
	glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);
	if (mPlacePosition.x >= x - 10) {
		direction = glm::vec3(cam->mCameraOffset.MaxX, 0.0f, 0.f);

	}
	if (mPlacePosition.x <= 10) {
		direction = glm::vec3(cam->mCameraOffset.minX, 0.0f, 0.f);

	}
	if (mPlacePosition.y >= y - 10) {
		direction = glm::vec3(0.0f, cam->mCameraOffset.minY, 0.f);

	}
	if (mPlacePosition.y <= 10) {
		direction = glm::vec3(0.0f, cam->mCameraOffset.MaxY, 0.f);

	}

	float xd = direction.x - cam->pos.x;
	float yd = direction.y - cam->pos.y;
	float distance = sqrt(xd * xd + yd * yd);
	float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
	if (length>=0.1f)
		direction = direction / length;
	cam->pos += (glm::vec2((direction.x), direction.y) * aDt * (distance*2.f));
	cam->pos.x = CLAMP(cam->pos.x, cam->mCameraOffset.MaxX, cam->mCameraOffset.minX);
	cam->pos.y = CLAMP(cam->pos.y, cam->mCameraOffset.MaxY, cam->mCameraOffset.minY);
	*/
}

int Player::GetAbilityCost()
{
	switch (mPlayerClass)
	{
	case 0:
	{
		return COSTBOMB;
	}
	case 1:
	{
		return COSTTELEPORT;
	}
	case 2:
	{
		return COSTEXTRAFIELDS;
	}
	case 3:
	{
		return COSTSELL;
	}
	default:
		printf("Something gone wrong");
		break;
	}
	return 0;
}

void Player::CallAbility(glm::vec2 aPosition)
{
	if (CanBuy(GetAbilityCost()) == true) //Check if the player has enough money to buy the ability
	{
		switch (mPlayerClass)
		{
		case(0):
		{
			BombAbility(aPosition);
			break;
		}
		case(1):
		{
			StartTeleportAbility(aPosition);
			break;
		}
		case(2):
		{
			PlaceExtraField(aPosition);
			break;
		}
		case(3):
		{
			SellTurrets(aPosition);
			break;
		}
		default:

			break;
		}
	}
}

void Player::CheckPlace(glm::vec2 aPosition, Entity* aEntity)
{
	while (AssignTurretPosition(aPosition, static_cast<Turret*>(aEntity)->GetRadiusSize()) == true)
	{
		int randX = rand() % 2 * 2 - 1;
		int randY = rand() % 2 * 2 - 1;
		aPosition = aPosition + glm::vec2(randX, randY);
	}
	static_cast<Turret*>(aEntity)->SetPosition(aPosition);
	AddBuilding(static_cast<Turret*>(aEntity));
	AddTurret(static_cast<Turret*>(aEntity));
	return;
}

void Player::HoverCheck(glm::vec2 aPosition)
{
	DebugRenderer::sInstance().clear();
	for (int i = 0; i < mCurrentBuildings.size(); i++)
	{
		Building* building = mCurrentBuildings[i];
		if (building != nullptr)
		{
			float distance = (powf(building->GetPosition().x - aPosition.x, 2.0f) + powf(building->GetPosition().y - aPosition.y, 2.0f));
			if (distance <= mCursorRadius * mCursorRadius)
			{
				building->SetHover(true);
			}
			else
			{
				building->SetHover(false);
			}
		}
	}
	DebugRenderer::sInstance().drawCircle(aPosition.x, aPosition.y, mCursorRadius, DebugColor::Black, CircleResolution::high);
	return;
}

void Player::BombAbility(glm::vec2 aPosition)
{
	Entity* b = EntitySystem::CreateEntity<BulletRocket>();
	static_cast<BulletRocket*>(b)->Init(BuildingInspectable::GetInstance()->ATTACKROCKET, 1, aPosition);
	//Rocket attack doesnt actually uses the entity as it is an explosion
	static_cast<BulletRocket*>(b)->Attack(nullptr);
}

void Player::StartTeleportAbility(glm::vec2 aPosition)
{
	for (int i = 0; i < mCurrentTurrets.size(); i++)
	{
		Turret* turret = mCurrentTurrets[i];
		if (turret != nullptr)
		{
			float distance = (powf(turret->GetPosition().x - aPosition.x, 2.0f) + powf(turret->GetPosition().y - aPosition.y, 2.0f));
			if (distance <= mCursorRadius * mCursorRadius)
			{
				mTurretToTeleport.push_back(turret);
			}
		}
	}
	if (mTurretToTeleport.size() > 0)
	{
		mStartTeleportPos = aPosition;
		mTeleportFlag = true;
	}
	else
	{
		mMoney += COSTTELEPORT;
	}
}

void Player::ExtraFieldsUpdate(float aDt)
{
	for (int i = 0; i < mExtraFields.size(); i++)
	{
		HoverCheck(mExtraFields[i]->mPosition);
		mExtraFields[i]->mTimer -= aDt;
		if (mExtraFields[i]->mTimer <= 0.f)
		{
			delete mExtraFields[i];
			mExtraFields.erase(mExtraFields.begin() + i);
		}
	}
}

void Player::EndTeleportAbility(glm::vec2 aPosition)
{
	if (mTeleportFlag)
	{
		mTeleportFlag = false;
		glm::vec3 direction = glm::vec3(glm::vec2(aPosition.x - mStartTeleportPos.x, aPosition.y - mStartTeleportPos.y), 0.f);
		float length = sqrt((direction.x * direction.x) + (direction.y * direction.y));
		if (length > 0.f)
		{
			direction = direction / length;
			for (int i = 0; i < mTurretToTeleport.size(); i++)
			{
				mTurretToTeleport[i]->mTransform.mPosition += (glm::vec2)(direction * length);
			}
		}
		else
		{
			mMoney += COSTTELEPORT;
		}
		mTurretToTeleport.clear();
	}
}

void Player::PlaceExtraField(glm::vec2 aPosition)
{
	ExtraField* extraField = new ExtraField(aPosition);
	mExtraFields.push_back(extraField);
}

void Player::SellTurrets(glm::vec2 aPosition)
{
	for (int i = 0; i < mCurrentTurrets.size(); i++)
	{
		Turret* turret = mCurrentTurrets[i];
		if (turret != nullptr)
		{
			float distance = (powf(turret->GetPosition().x - aPosition.x, 2.0f) + powf(turret->GetPosition().y - aPosition.y, 2.0f));
			if (distance <= mCursorRadius * mCursorRadius)
			{
				turret->SellTurret();
			}
		}
	}
}

Player::~Player()
{
	delete mPlayerInspector;
}

/// <summary>
/// Function to use when a player wants to buy a new building(turrets or tower)
/// </summary>
/// <param name="aBuilding"></param>
bool Player::CanBuy(int aCost)
{
	if (gInfiniteMoney)
		return true;
	if (mMoney > aCost)
	{
		printf("you have enough money to buy ");
		mMoney -= aCost;
		return true;
	}
	else
	{
		printf("you don't have enough money to buy ");
		return false;
	}
}

void Player::Update(float aDt) // Don't add the aDeltaTime if not using this is a better way to get rid of the warning from unused variable on ps4
{
	InputUpdate(aDt);

	CameraControl(aDt);
	Camera2D* cam = divil::GetCamera2D();
	if (divil::Input::ButtonPressed(divil::AbstractControlIndex_X))
	{
		printf("Screenshakey\n");
		cam->BeginShake(0.2f, 0.02f);
	}

	cam->Update(aDt);

}

int Player::GetBuildingCost(int aBuildingToBuy)
{
	switch (aBuildingToBuy)
	{
	case 0:
	{
		return COSTMACHINEGUN;
	}
	case 1:
	{
		return COSTMINIGUN;
	}
	case 2:
	{
		return BuildingInspectable::GetInstance()->COSTSHOTGUN;
	}
	case 3:
	{
		return BuildingInspectable::GetInstance()->COSTROCKET;
	}
	case 4:
	{
		return COSTSNIPER;
	}
	default:
		printf("Something gone wrong");
		break;
	}
	return 0;
}

int Player::GetBuildingCost(std::string aName)
{
	if(aName == "Machinegun")
	{
		return COSTMACHINEGUN;
	}
	else if (aName == "Minigun")
	{
		return COSTMINIGUN;
	}
	else if (aName == "Shotgun")
	{
		return BuildingInspectable::GetInstance()->COSTSHOTGUN;
	}
	else if (aName == "Rocket")
	{
		return BuildingInspectable::GetInstance()->COSTROCKET;
	}
	else if (aName == "Sniper")
	{
		return COSTSNIPER;
	}
	else
	{
		printf("Something gone wrong");
		return 0;
	}

}

bool Player::AssignTurretPosition(glm::vec2 aPosition, float aRadius)
{
	float x1 = aPosition.x, y1 = aPosition.y;
	float r1 = aRadius;
	for (int i = 0; i < mCurrentBuildings.size(); i++)
	{
		float x2 = mCurrentBuildings[i]->GetPosition().x, y2 = mCurrentBuildings[i]->GetPosition().y;
		float r2 = mCurrentBuildings[i]->GetRadiusSize();
		bool intersectionFlag = Circle(x1, y1, x2, y2, r1, r2);
		if (intersectionFlag == true)
		{
			return true;
		}
	}
	return false;
}

bool Player::Circle(float x1, float y1, float x2, float y2, float r1, float r2)
{
	float x = x1 - x2;
	float y = y1 - y2;
	float distance = x * x + y * y;
	float intersectDistance = (r1 + r2) * (r1 + r2);

	return distance <= intersectDistance;

}