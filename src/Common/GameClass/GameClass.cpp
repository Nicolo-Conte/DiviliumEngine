#include "GameClass/GameClass.h"
#include "StateMachine/StateMachine.h"
#include "GameStates/EndScreenState.h"
#include "GameStates/GameLoopState.h"
#include "GameStates/HomeScreenState.h"
#include "Enemy/EnemyWaveSystem.h"
#include "player/Player.h"
#include "ResearchSystem/ResearchSystem.h"
#include "GameStates/ClassSelectState.h"
#include "Entities/Collision.h"
#include "BackGround/TileManager.h"
#include "BackGround/TileMapManager.h"
#include "Divilium.h"
#include "DebugTools/EntitySpawner.h"
#include "rendering/Camera2D.h"
#include "Enemy/Enemy.h"
GameClass* GameClass::mGameClass = nullptr;

void GameClass::InitializeGameStates()
{
    ClassSelectState* classSelect = new ClassSelectState();
    HomeScreenState* homeScreen = new HomeScreenState();
    EndScreenState* endScreen = new EndScreenState();
    GameLoopState* gameLoop = new GameLoopState();

    classSelect->Init();
    homeScreen->Init();
    endScreen->Init();
    gameLoop->Init();

    mGameStates = new StateMachine();
    
    mGameStates->AddState(classSelect);
    mGameStates->AddState(gameLoop);
    mGameStates->AddState(homeScreen);
    mGameStates->AddState(endScreen);
    mGameStates->ChangeState(homeScreen->mName);
}

GameClass::GameClass()
{
    InitializeGameStates();
}

GameClass* GameClass::GetInstance()
{
    if (mGameClass == nullptr)
        mGameClass = new GameClass();
    return mGameClass;
}

void GameClass::Init()
{
    getEnemyInspector();
    BuildingInspectable::GetInstance();
    //Setting the input
    StartButton = divil::Input::CreateButtonMapping();
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(),divil::AbstractControlIndex_R, StartButton);
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_LPad_Up, StartButton);

    PlaceButton = divil::Input::CreateButtonMapping();
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_LButton, PlaceButton);
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_RPad_Down, PlaceButton);

    RightButton = divil::Input::CreateButtonMapping();
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_Right, RightButton);
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_LPad_Right, RightButton);

    LeftButton = divil::Input::CreateButtonMapping();
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_Left, LeftButton);
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_LPad_Left, LeftButton);

    AbilityButton = divil::Input::CreateButtonMapping();
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_RButton, AbilityButton);
    divil::Input::MapButton(divil::Input::DefaultControlMappingTable(), divil::AbstractControlIndex_RPad_Left, AbilityButton);

    //creating collision layers
    
    int tBuilding =  Collision::CreateCollisionLayerWithName("Buildings");
    int tEnemy =  Collision::CreateCollisionLayerWithName("Enemies");
    int tBullet =  Collision::CreateCollisionLayerWithName("Bullets");
    int tEBullet = Collision::CreateCollisionLayerWithName("EnemyBullets");
    int tMine =  Collision::CreateCollisionLayerWithName("Mines");
    int tSpawner =  Collision::CreateCollisionLayerWithName("EnemySpawner");
    int Tile = Collision::CreateCollisionLayerWithName("Tiles");
    
    //dont collide with the tiles
    Collision::SetCollisionLayerCondition(Tile, tBuilding, false);
    Collision::SetCollisionLayerCondition(Tile, tEnemy, false);
    Collision::SetCollisionLayerCondition(Tile, tBullet, false);
    Collision::SetCollisionLayerCondition(Tile, tMine, false);
    Collision::SetCollisionLayerCondition(Tile, tSpawner, false);
    Collision::SetCollisionLayerCondition(Tile, Tile, false);

    //building collision
    Collision::SetCollisionLayerCondition(tBuilding, tMine, false);
    Collision::SetCollisionLayerCondition(tBuilding, tSpawner, false);
    Collision::SetCollisionLayerCondition(tBuilding, tBuilding, false);
    
    //enemy collision
    Collision::SetCollisionLayerCondition(tEnemy, tSpawner, false);
    Collision::SetCollisionLayerCondition(tEnemy, tEnemy, false);

    //bullet collision
    Collision::SetCollisionLayerCondition(tBullet, tSpawner, false);
    Collision::SetCollisionLayerCondition(tBullet, tMine, false);
    Collision::SetCollisionLayerCondition(tBullet, tBuilding, false);
    Collision::SetCollisionLayerCondition(tBullet, tBullet, false);

    //enemy bullet collision
    Collision::SetCollisionLayerCondition(tEBullet, tEBullet, false);
    Collision::SetCollisionLayerCondition(tEBullet, tBullet, false);
    Collision::SetCollisionLayerCondition(tEBullet, tSpawner, false);
    Collision::SetCollisionLayerCondition(tEBullet, tMine, false);
    
    //mine colision
    Collision::SetCollisionLayerCondition(tMine, tSpawner, false);

    //init other functions
    EnemyWaveSystem::Init();
    //get an first instance so the player gets created
    Player::GetInstance();
    mResearchSystem = new ResearchSystem();
    mEntitySpawner = new EntitySpawner();

    //Map settings
      //set world size
    float wX = 1920;
    float wY = 1080;

    float tileSize = 64;
    divil::SetWorldMinMax(glm::vec2 (-wX/2.f,-wY/2.f),glm::vec2(wX/2.f, wY/2.f));
    divil::TileMapManager* mMap = divil::TileMapManager::GetInstance();

    mMap->SetTileSize(tileSize);
    mMap->SetFrequency(0.02);
    mMap->SetOcaves(4);
    mMap->SetPersistance(4);
    

    divil::TileManager* tileManager = mMap->GetTileManager();
    tileManager->setDefault(divil::TileData("Default", 1, glm::vec3(0, 1, 0)));
    tileManager->AddNewTile(divil::TileData("Water", 5, glm::vec3(0, 0, 1)),0.65f);
}

GameClass::~GameClass()
{
    delete mGameStates;
}

StateMachine* GameClass::GetGameStates()
{
    return mGameStates;
}

bool GameClass::Update(float aDt)
{
    //gamestates returns true if it is in a state that needs to update things like the waves and other things
    
    bool returnval = mGameStates->Update(aDt);
    if (returnval)
    {
        mEntitySpawner->Update();
        EnemyWaveSystem::Update(aDt);
        Player::GetInstance()->Update(aDt);
    }

    return returnval;
}