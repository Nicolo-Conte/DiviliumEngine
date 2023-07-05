#include "DebugTools/EntitySpawner.h"
#include "GameClass/GameClass.h"
#include"StateMachine/StateMachine.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Player/Player.h"
#include "Divilium.h"
#include "rendering/Camera2D.h"
#include "DebugUI/ImGuiHandler.h"
#include "Entities/EntitySystem.h"
#include "Building/Mine.h"
#include "Player/CoreManager.h"
#include "DebugUI/ImGuiHandler.h"
EntitySpawner::EntitySpawner()
{
    SetName("EntitySpawner");
    //The order is important to the order of the Spawning function
    AddItem("Enemy_Transporter",Kind_EnemyTransporter);
    AddItem("Enemy_Sniper", Kind_EnemySniper);
    AddItem("Enemy_Tank",Kind_EnemyTank);
    AddItem("Enemy_Kamikaze",Kind_EnemyKamikaze);
    AddItem("Building_Core",Kind_Core);
    AddItem("Building_Mine",Kind_Mine);
    AddItem("Turret_Machinegun",Kind_Turret);
    AddItem("Turret_Minigun",Kind_Turret);
    AddItem("Turret_Shotgun",Kind_Turret);
    AddItem("Turret_RPG", Kind_Turret);
    AddItem("Turret_Sniper", Kind_Turret);
    mActive = false;
}

void EntitySpawner::Display()
{
    if (GameClass::GetInstance()->GetGameStates()->GetCurrentName() == "GameLoopState")
    {
    
        glm::vec2 pos = MousePos();
        pos = divil::GetCamera2D()->ToWorldSpace(pos);
        
        ImGui::Text("World Position");
        ImGui::Text("X: %f, Y: %f", pos.x, pos.y);
        ImGui::Spacing();
        ImGui::Text("Select Entity Type");
        ImGui::PushItemWidth(150);

        ImGui::ListBox("Type", &currentItem, UIlist, ListSize);
        ImGui::Spacing();
        ImGui::InputInt("amount to spawn in one click", &SpawnAmount);
        if (mActive)
        {
            if (ImGui::Button("Disable Spawning"))
            {
                mActive = false;
                Player::GetInstance()->SetMouseClickActive(!mActive);
            }
        }
        else
        {
            if (ImGui::Button("Enable Spawning"))
            {
                mActive = true;
                Player::GetInstance()->SetMouseClickActive(!mActive);
            }
        }
    }
}

void EntitySpawner::AddItem(const char* aName, Kind aKind)
{
    if (ListSize >= MaxSize)
        return;
    UIlist[ListSize] = aName;
    KindList[ListSize] = aKind;
    ListSize++;
}
glm::vec2 EntitySpawner::MousePos()
{
    int mX, mY, sX, sY;
    divil::Input::GetMousePosition(&mX, &mY);
   glm::vec2 pos = divil::ConvertScreenPosToGameScreen(glm::vec2(mX, mY));
    divil::GetWindowSize(&sX, &sY);
    
    Camera2D* mCamera =divil::GetCamera2D();
    return glm::vec2(pos.x + (-mCamera->pos.x * sX / 2), pos.y + (mCamera->pos.y * sY / 2));
}
void EntitySpawner::Update()
{
    if (mActive)
    {
        if (divil::Input::ButtonPressed(divil::AbstractControlIndex_LButton) && !divil::ImGuiHandler::IsAWindowHovered() )
        {
            Spawn(currentItem, MousePos());
        }

    }
}
void EntitySpawner::Spawn(int index,glm::vec2 Place)
{   
    for (int i = 0; i < SpawnAmount; i++)
    {
        switch (index)
        {
        case 0:
            EnemyWaveSystem::SpawnEnemy(KindList[index], Place);
            break;
        case 1:
            EnemyWaveSystem::SpawnEnemy(KindList[index], Place);
            break;
        case 2:
            EnemyWaveSystem::SpawnEnemy(KindList[index], Place);
            break;
        case 3:
            EnemyWaveSystem::SpawnEnemy(KindList[index], Place);
            break;
        case 4:
            Player::GetInstance()->GetCoreManager()->Place(Place);
            break;
        case 5:
            EntitySystem::CreateEntity<Mine>(Place);
            break;
        case 6:
            //machinegun
            Player::GetInstance()->PlaceBuilding(0, Place);
            break;
        case 7:
            //Turret_Minigun
            Player::GetInstance()->PlaceBuilding(1, Place);
            break;
        case 8:
            //Turret_Shotgun
            Player::GetInstance()->PlaceBuilding(2, Place);
            break;
        case 9:
            //Turret_RPG
            Player::GetInstance()->PlaceBuilding(3, Place);
            break;
        case 10:
            //Turret_Sniper
            Player::GetInstance()->PlaceBuilding(4, Place);
            break;
        case 11:
            //machinegun
            Player::GetInstance()->PlaceBuilding(5, Place);
            break;
        case 12:
            break;
        }
    }
}
