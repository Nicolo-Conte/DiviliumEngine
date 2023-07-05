#include "src\RTSPrecomp.h"
#include "Enemy/EnemyWaveSystem.h"
#include "Entities/EntitySystem.h"
#include "GameClass/GameClass.h"
#include "Enemy/Enemy.h"
#include "Logger/Log.h"
#include "Enemy/EnemySpawner.h"
#include "ResearchSystem/ResearchSystem.h"
#include "ResearchSystem/ResearchManager.h"
#include "StateMachine/StateMachine.h"
#include "DifficultyCurve/DifficultyCurve.h"
#include "DifficultyCurve/CurveWidget.h"
#include "Serialize/Serialize.h"

//all of the enemy types
#include "Enemy/EnemySoldier.h"
#include "Enemy/EnemyTransporter.h"
#include "Enemy/EnemyKamikaze.h"
#include "Enemy/EnemySniper.h"
#include "Enemy/EnemyTank.h"


using namespace divil;

//all enemies, timers and Enemyspawners should be registered
static std::vector<EntityHandle> gEnemies;
static std::vector<EntityHandle> gEnemySpawners;
static std::vector<Timing> gTimer;
//the distance around a point that an enemy can spawn
static glm::vec2 gMinDistance(150,150);
static glm::vec2 gMaxDistance(200,200);
static glm::vec2 gSpawnAround(0,0);

//spawn chance relative to eachother
static int gKamikazeSC = 10;
static int gTankSC = 10;
static int gTransPorterSC = 10;
static int gSniperSC = 10;
static int gDefaultSC = 60;

//current wave
static int gWave = 0;
//these two booleans are to make sure the waves end when 
static bool gFinalWave = false;
static bool gNoWaves = false;
//determents what will be spawned per wave

static DifficultyCurve* gDifficultyCurve;
static divil::Array<ManualWave> gManualWaves;
//variables that change
static float gPredictTime = 1.f;
static int gSpawnChance = 100;


//this is for the debugUI
class EnemyWaveInspector;
EnemyWaveInspector* gWaveInspector;
static bool gDebugWavesOn = true;

//saving changes
bool gUnsavedChange = false;
float gSaveTime = 0;
float gTimeTillSave = 10.f;
void SerializeEnemyWaves(divil::SerializeMode aMode)
{
    divil::Serializer ser;
    if (BeginSerializeFile(ser, "EnemyWaves.data", 0, aMode))
    {
        Serialize(ser, *gDifficultyCurve);
        Serialize(ser, gManualWaves);
        Serialize(ser, gKamikazeSC);
        Serialize(ser, gTankSC);
        Serialize(ser, gTransPorterSC);
        Serialize(ser, gSniperSC);
        Serialize(ser, gDefaultSC);
        Serialize(ser, gMinDistance.x);
        Serialize(ser, gMinDistance.y);
        Serialize(ser, gMaxDistance.x);
        Serialize(ser, gMaxDistance.y);
        EndSerializeFile(ser);
    }
}
void Save()
{
    SerializeEnemyWaves(divil::SerializeMode_Write);
}
void UpdateSave(float aDT)
{
    if (gUnsavedChange)
    {
        gSaveTime -= aDT;
        if (gSaveTime < 0)
        {
            Save();
            gUnsavedChange = false;
        }
    }
}


class EnemyWaveInspector :
    public Inspectable
{
    //these display the different types of enemies to the UI

    static const int mSize = 10;
    const char* UIList[mSize]{};
    Kind UImap[mSize]{};
    int currentItem = 0;
    int ListSize = 0;

    //these are for creating new manual waves
    bool mOveride = false;
    float mTime = 0;
    int mWaves = 0;
    int mKind = 0;
    int mAmount = 0;

    //these are for the wave points
    Point EndPoint;
    Point BeginPoint;
    Point NewPoint;

    void Settings()
    {
        ImGui::Text("Enemy relative Spawn Chance");
        ImGui::InputInt("Kamikaze", &gKamikazeSC);
        ImGui::InputInt("Sniper", &gSniperSC);
        ImGui::InputInt("Tank", &gTankSC);
        ImGui::InputInt("Transporter", &gTransPorterSC);
        ImGui::InputInt("Default", &gDefaultSC);
        ImGui::Spacing();
        ImGui::Text("Spawn distance around the main tower");
        float v[2] = {gMinDistance.x,gMinDistance.y};
        if (ImGui::InputFloat2("Min", v))
        {
            gMinDistance.x = v[0];
            gMaxDistance.y = v[1];
            Save();
        }
        
        v[0] = gMaxDistance.x;
        v[1] = gMaxDistance.y;
        if (ImGui::InputFloat2("Max", v))
        {
            gMaxDistance.x = v[0];
            gMaxDistance.y = v[1];
            Save();
        }
    }
    void EnableAutoSave()
    {
        gSaveTime = gTimeTillSave;
        gUnsavedChange = true;
    }
    void reorderManualWaves()
    {
        int i;
        int j;
        if (gManualWaves.Count() == 0)
            return;
        for (i = 0; i < gManualWaves.Count() - 1; i++)
        {
            for (j = 0; j < gManualWaves.Count() - i - 1; j++)
            {
                if (gManualWaves[j].wave > gManualWaves[j + 1].wave)
                {
                    ManualWave temp = gManualWaves[j];
                    gManualWaves[j] = gManualWaves[j + 1];
                    gManualWaves[j + 1] = temp;
                }
            }
        }
    }
    void DisplayManualWave(int* wave, bool& overide, int& kind, int* amount, float* Time)
    {
        int Item;
        for (int i = 0; i < mSize; i++)
        {
            if (UImap[i] == kind)
            {
                Item = i;
                break;
            }
        }
        if (ImGui::InputInt("wave :", wave))
        {
            EnableAutoSave();
        }

        if (ImGui::Checkbox("Overide", &overide))
        {
            EnableAutoSave();
        }
        if (ImGui::ListBox("Current Time To spawn", &Item, UIList, ListSize))
        {
            EnableAutoSave();
        }
        kind = UImap[Item];
        if (ImGui::InputInt("amount", amount))
        {
            EnableAutoSave();
        }
        if (ImGui::InputFloat("Timing : ", Time))
        {
            EnableAutoSave();
        }
    }
    void ManualWaves()
    {
        if (ImGui::Button("Reorganize"))
        {
            reorderManualWaves();
            Save();
        }
        for (int i = 0; i < gManualWaves.Count(); i++)
        {
            ImGui::PushID(i);
            std::string wave("Wave : ");
            wave.append(std::to_string(gManualWaves[i].wave));
            if (ImGui::CollapsingHeader(wave.c_str(), gManualWaves[i].ImguiHeader))
            {

                if (ImGui::Button("Remove Item"))
                {
                    gManualWaves.RemoveOrdered(i);
                    Save();
                }
                else
                {
                    int temp = gManualWaves[i].data.kind;
                    DisplayManualWave(&gManualWaves[i].wave, gManualWaves[i].overide, temp, &gManualWaves[i].data.amount, &gManualWaves[i].Time);
                    gManualWaves[i].data.kind = temp;
                }

            }
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("add new Item"))
        {
            DisplayManualWave(&mWaves, mOveride, mKind, &mAmount, &mTime);
            if (ImGui::Button("Create item"))
            {
                ManualWave manualWave(mWaves, mOveride);
                manualWave.data.kind = mKind;
                manualWave.data.amount = mAmount;
                manualWave.Time = mTime;
                gManualWaves.Add(manualWave);
                Save();
            }
        }

    }

    void DisplayDifficultyCurve()
    {

        int ArrSize = gDifficultyCurve->GetBezierSize() - 1;
        int offset = 0;
        for (int i = 0; i < ArrSize; i++)
        {
            ImGui::PushID(i);
            BezierPoint* Begin = gDifficultyCurve->GetBezierPoint(i + offset);
            BezierPoint* End = gDifficultyCurve->GetBezierPoint(i + 1 + offset);

            std::string point("From : ");
            point.append(std::to_string(Begin->mBegin.x));
            point.append(" To : ");
            point.append(std::to_string(End->mBegin.x));

            if (ImGui::CollapsingHeader(point.c_str()))
            {
                if (ImGui::Button("Remove Wave"))
                {
                    gDifficultyCurve->RemovePoint(i);
                    //since the item got removed everything got shifted to the left
                    //so add an offset
                    offset--;
                }
                else
                {
                    BeginPoint = Begin->mBegin;
                    ImGui::Text("Begin Point : ");
                    if (ImGui::InputFloat("Begin X :", &BeginPoint.x))
                    {
                        Begin->mBegin = BeginPoint;
                        gDifficultyCurve->BezierReorderList();
                        EnableAutoSave();
                    }
                    if (ImGui::InputFloat("Begin Y :", &BeginPoint.y))
                    {
                        Begin->mBegin = BeginPoint;
                        EnableAutoSave();
                    }

                    EndPoint = End->mBegin;
                    ImGui::Text("End Point : ");
                    if (ImGui::InputFloat("End X :", &EndPoint.x))
                    {
                        gDifficultyCurve->BezierReorderList();
                        End->mBegin = EndPoint;
                        EnableAutoSave();
                    }
                    if (ImGui::InputFloat("End Y :", &EndPoint.y))
                    {
                        End->mBegin = EndPoint;
                        EnableAutoSave();
                    }

                    //used to normalize
                    float maxX = glm::max(Begin->mBegin.x, End->mBegin.x);
                    float minX = glm::min(Begin->mBegin.x, End->mBegin.x);

                    //display
                    float v[2];
                    v[0] = Begin->mGradient.x;
                    v[1] = Begin->mGradient.y;
                    ImVec2 Position;
                    ImGui::Bezier(point.c_str(), v, &Position);
                    //make sure that the x stays within its bounds
                    float x = glm::clamp(v[0], 0.f, 1.f);
                    float y = glm::clamp(v[1], -0.5f, 1.5f);
                    if (x != Begin->mGradient.x || y != Begin->mGradient.y)
                    {
                        Begin->mGradient.x = x;
                        Begin->mGradient.y = y;
                        EnableAutoSave();
                    }
                    //show the current position
                    Position.x = Position.x * (maxX - minX) + minX;
                    float val = 0;
                    gDifficultyCurve->GetBezierValue(Position.x, val);
                    ImGui::Text("current pos X : %d, Y: %d \n", (int)Position.x, (int)val);
                }
            }
            ImGui::PopID();
        }
        if (ImGui::CollapsingHeader("new Point"))
        {
            ImGui::InputFloat("X :", &NewPoint.x);
            ImGui::InputFloat("Y :", &NewPoint.y);
            if (ImGui::Button("add Point"))
            {
                gDifficultyCurve->AddBezierPoint(NewPoint);
                Save();
            }
        }
    }

public:
    EnemyWaveInspector()
    {
        SetName("WaveSystem");
    }

    void Display() override
    {
        if (ImGui::Button("Save"))
            Save();
        ImGui::Checkbox("toggle waves", &gDebugWavesOn);
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Settings"))
            Settings();
        ImGui::Dummy(ImVec2(0, 5));

        if (ImGui::CollapsingHeader("DifficultyCurve"))
            DisplayDifficultyCurve();
        ImGui::Dummy(ImVec2(0, 5));

        if (ImGui::CollapsingHeader("Manual waves"))
            ManualWaves();
        ImGui::Dummy(ImVec2(0, 5));
        if (GameClass::GetInstance()->GetGameStates()->GetCurrentName() == "GameLoopState")
        {
            if (ImGui::CollapsingHeader("Random Enemy Spawning"))
            {
                ImGui::Spacing();
                ImGui::Text("Select Enemy Type");
                ImGui::PushItemWidth(100);

                ImGui::ListBox("Type", &currentItem, UIList, ListSize);
                ImGui::Spacing();
                if (ImGui::Button("spawn enemy"))
                {
                    EnemyData data;
                    data.kind = UImap[currentItem];
                    data.amount = 1;
                    EnemyWaveSystem::SpawnEnemyRandom(data);
                }
            }
        }
    }

    void AddEnemyType(const char* name, Kind type)
    {
        UIList[ListSize] = name;
        UImap[ListSize] = type;
        ListSize++;
    }
};








int EnemyWaveSystem::Update(float aDt)
{
    UpdateSave(aDt);
    //check if the waves have been paused by the debug UI
    if (gDebugWavesOn && !gNoWaves)
    {
        //if a wave is over start a new one
        if (WaveOver())
        {
            if (gFinalWave)
            {
                gNoWaves = true;
                GameClass::GetInstance()->getResearchSystem()->Open();
            }
            else
            {
                gWave += 1;
                CreateWave(gWave);
            }
        }
        else
        {
            //go over the timers
            for (int i = 0; i < gTimer.size(); i++)
            {
                //if the timer reaches one second
                //the timer will become an enemySpawner instead
                gTimer[i].Time -= aDt;
                if (gTimer[i].Time < gPredictTime)
                {
                    int Spawn = rand() % 100;
                    if (Spawn < gSpawnChance)
                    {
                        CreateEnemySpawner(gTimer[i]);
                    }
                 gTimer.erase(gTimer.begin() + i);
                }
            }
        }
    }
    return 0;
}
//the wave is over once the enemies are dead and there is no timer left
bool EnemyWaveSystem::WaveOver()
{
    return (gEnemies.empty() && gTimer.empty() && gEnemySpawners.empty());
}
//remove an enemy spawner or enemy
bool EnemyWaveSystem::RemoveItem(EntityHandle aHandle)
{
    //if it is the type of enemy spawner loop over the Spawner array 
    //else loop over the enemy array
    if(EntitySystem::GetEntity(aHandle)->GetKind() == Kind_EnemySpawner)
    {
        for (int i = 0; i < gEnemySpawners.size(); i++)
        {
            if (gEnemySpawners[i].generation == aHandle.generation
                && gEnemySpawners[i].index == aHandle.index)
            {
                gEnemySpawners.erase(gEnemySpawners.begin() + i);
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < gEnemies.size(); i++)
        {
            if (gEnemies[i].generation == aHandle.generation
                && gEnemies[i].index == aHandle.index)
            {
                gEnemies.erase(gEnemies.begin() + i);
                return true;
            }
        }
    }
    return false;
}
//add a timer to the current wave
int EnemyWaveSystem::AddTimer(Timing aTime)
{
    gTimer.push_back(aTime);
    return 0;
}
//create an enemy spawner
int EnemyWaveSystem::SpawnEnemy(glm::vec2 aPos, Timing aTiming)
{
    gEnemySpawners.push_back(EntitySystem::CreateEntity<EnemySpawner>(aTiming, aPos.x, aPos.y)->mHandle);
    return 0; 
}
//spawn the enemies based on the timer
//the timer holds the time that the spawner has to wait
//and the data which holds which types it should spawn and how many
//position will be randomized
int EnemyWaveSystem::CreateEnemySpawner(Timing aTiming)
{
    int sgnX = rand() % 2 * 2 - 1;
    int sgnY = rand() % 2 * 2 - 1;
    float r1 = static_cast<float>(rand()) / RAND_MAX;
    float r2 = static_cast<float>(rand()) / RAND_MAX;
    glm::vec2 basePos = gSpawnAround;
    basePos.x += static_cast<float>(sgnX) * (gMinDistance.x + r1 * (gMaxDistance.x - gMinDistance.x));
    basePos.y += static_cast<float>(sgnY) * (gMinDistance.y + r2 * (gMaxDistance.y - gMinDistance.y));
    gEnemySpawners.push_back(EntitySystem::CreateEntity<EnemySpawner>(aTiming, basePos.x, basePos.y)->mHandle);
  
    return 0;
}
//create the currtent wave
//you can set what enemies should spawn with manual waves
//orherwise it gets set trough the difficulty curve which can be set as well
int EnemyWaveSystem::CreateWave(int aWave)
{
    //reset the variables for the skill tree
    gFinalWave = false;
    gNoWaves = false;

    if (aWave == -1)
    {
        UpdateVariables();
        gWave++;
    }
    else
    gWave = aWave;
  
    if (!OverideSpawn(gWave))
        DefaultSpawn(gWave);

    return 0;
}
// get a single enemy
//returns a nullptr if it is not there
Enemy* EnemyWaveSystem::getEnemy(int aPlace)
{
    if (aPlace >= gEnemies.size())
        return nullptr;

    Entity* entity = EntitySystem::GetEntity(gEnemies[aPlace]);
    if (entity)
        return static_cast<Enemy*>(entity);

    return nullptr;
}
// get the amount of enemies alive
int EnemyWaveSystem::GetEnemyVecSize()
{
    return static_cast<int>(gEnemies.size());
}
//get the vector of all the enemies
std::vector<EntityHandle>& EnemyWaveSystem::GetEnemies()
{
    return gEnemies;
}
//This is the minimum and maximum distance an enemy spawner will be capable of spawning around a point set by SpawnAround
void EnemyWaveSystem::SpawnDistance(glm::vec2 aMinDistance, glm::vec2 aMaxDistance)
{
    gMinDistance = glm::vec2(glm::min(aMinDistance.x, aMaxDistance.x), glm::min(aMinDistance.y, aMaxDistance.y));
    gMaxDistance = glm::vec2(glm::max(aMinDistance.x, aMaxDistance.x), glm::max(aMinDistance.y, aMaxDistance.y));
}
//this is the point the enemy spawner will spawn around. The distance from that point can be set by SpawnDistance
void EnemyWaveSystem::SpawnAround(glm::vec2 aSpawnAround)
{
    gSpawnAround = aSpawnAround;
}
//remove all of the enemies, enemyspawner and timers
void EnemyWaveSystem::Clear()
{
    for (auto& gEnemySpawner : gEnemySpawners)
    {
        EntitySystem::RemoveEntity(EntitySystem::GetEntity(gEnemySpawner));
    }	
    gEnemySpawners.clear();

    for (auto& enemy : gEnemies)
    {
        EntitySystem::RemoveEntity(EntitySystem::GetEntity(enemy));
    }
    gEnemies.clear();

	gTimer.clear();
}
//add a singular enemy handle to the list
void EnemyWaveSystem::AddEnemy(EntityHandle aHandle)
{
    gEnemies.push_back(aHandle);
}
//spawn an enemy at a random point
void EnemyWaveSystem::SpawnEnemyRandom(EnemyData aData)
{
    //create a random position
    int sgnX = rand() % 2 * 2 - 1;
    int sgnY = rand() % 2 * 2 - 1;
    float r1 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float r2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    glm::vec2 basePos = gSpawnAround;
    basePos.x += static_cast<float>(sgnX) * (gMinDistance.x + r1 * (gMaxDistance.x - gMinDistance.x));
    basePos.y += static_cast<float>(sgnY) * (gMinDistance.y + r2 * (gMaxDistance.y - gMinDistance.y));
    for (int i = 0; i < aData.amount; i++)
    {

        //TODO when we are using non walkable tiles yo should not be capable of spawning there
        //add an offset to the position so the enemies wont be overlapping
    	sgnX = rand() % 2 * 2 - 1;
    	sgnY = rand() % 2 * 2 - 1;
        glm::vec2 pos = basePos;
        pos.x += static_cast<float>(i) * 5.f * sgnX;
        pos.y += static_cast<float>(i) * 5.f * sgnY;
        SpawnEnemy(aData.kind, pos);
    }
}

void EnemyWaveSystem::SpawnEnemy(uint32_t aKind, glm::vec2 aPos)
{
    Enemy* enemy;
    //switch based on the enemy type
    switch (aKind)
    {
    default:
    {
        assert("tried to spawn a non enemy with the wave system");
        return;
    }
    case Kind_Enemy:
    {
        enemy = static_cast<Enemy*>(EntitySystem::CreateEntity<EnemySoldier>());

        break;
    }
    case Kind_EnemyTransporter:
    {
        enemy = static_cast<Enemy*>(EntitySystem::CreateEntity<EnemyTransporter>());
        break;
    }
    case Kind_EnemyKamikaze:
    {
        enemy = static_cast<Enemy*>(EntitySystem::CreateEntity<EnemyKamikaze>());
        break;
    }
    case Kind_EnemySniper:
    {
        enemy = static_cast<Enemy*>(EntitySystem::CreateEntity<EnemySniper>());
        break;
    }
    case Kind_EnemyTank:
    {
        enemy = static_cast<Enemy*>(EntitySystem::CreateEntity<EnemyTank>());
        break;
    }
 
    }
    enemy->SetPosition(aPos);
    gEnemies.push_back(enemy->mHandle);
}

void EnemyWaveSystem::FinalWave()
{
    gFinalWave = true;
}

void EnemyWaveSystem::UpdateVariables()
{
    gSpawnChance = glm::clamp(static_cast<int>(GameClass::GetInstance()->getResearchSystem()->mResearchManager->GetVariable("SpawnChance")),50,100);
    gPredictTime = GameClass::GetInstance()->getResearchSystem()->mResearchManager->GetVariable("PredictTime");
}




void EnemyWaveSystem::DefaultSpawn(int aWave)
{
    //get everything you want to spawn
    float Value = static_cast<float>(aWave * aWave + 1);
     gDifficultyCurve->GetBezierValue(static_cast<float>(aWave),Value);
   
    int ToSpawn = static_cast<int>(Value);
    printf("Current Wave %d \n", aWave);
    printf("Value %d\n", ToSpawn);

    int EnemiesLeft = ToSpawn;
    Timing time;
    
    int Spawners = 1;
    if (ToSpawn > 20)
        Spawners = rand() % 4 + 1;
    int MaxValue = gKamikazeSC + gSniperSC + gDefaultSC + gTransPorterSC + gTankSC;
    for (int j = 0; j < Spawners; j++)
    {

        for (int i = 0; i < ToSpawn/Spawners; i++)
        {
            int Rand = rand() % MaxValue;
            if (Rand <= gKamikazeSC)
            {
                EnemyData data2;
                data2.amount = 1;
                data2.kind = Kind_EnemyKamikaze;
                time.data.push_back(data2);
                EnemiesLeft--;
                continue;
            }
            else if (Rand <= gSniperSC + gKamikazeSC)
            {
                EnemyData data2;
                data2.amount = 1;
                data2.kind = Kind_EnemySniper;
                time.data.push_back(data2);
                EnemiesLeft--;
                continue;
            }
            else if (Rand <= gTankSC + gSniperSC + gKamikazeSC)
            {
                EnemyData data2;
                data2.amount = 1;
                data2.kind = Kind_EnemyTank;
                time.data.push_back(data2);
                EnemiesLeft--;
                continue;
            }
            else if (Rand <= gTransPorterSC + gTankSC + gSniperSC + gKamikazeSC)
            {
                EnemyData data2;
                data2.amount = 1;
                data2.kind = Kind_EnemyTransporter;
                time.data.push_back(data2);
                EnemiesLeft--;
                continue;
            }
        }
        EnemyData data;
        data.amount = EnemiesLeft/Spawners;
        data.kind = Kind_Enemy;
        time.data.push_back(data);

        //take a random value between 1 and 4 seconds
        time.Time = static_cast<float>(rand() % 4 + 1);
        AddTimer(time);
    }

}

bool EnemyWaveSystem::OverideSpawn(int aWave)
{
    bool returnVal = false;
    std::vector<Timing> tempTiming;
    for (int i = 0; i < gManualWaves.Count(); i++)
    {

        if (gManualWaves[i].wave == aWave)
        {
            if (gManualWaves[i].overide)
                returnVal = true;

            bool found = false;
            for (int j = 0; j < tempTiming.size(); j++)
            {
                if (tempTiming[j].Time == gManualWaves[i].Time)
                {
                    found = true;
                    tempTiming[j].data.push_back(gManualWaves[i].data);
                    break;
                }
            }
            if (!found)
            {
                Timing timing;
                timing.data.push_back(gManualWaves[i].data);
                timing.Time = gManualWaves[i].Time;
                tempTiming.push_back(timing);
            }
        }
    }
    for (int i = 0; i < tempTiming.size(); i++)
    {
        AddTimer(tempTiming[i]);
    }
    return returnVal;
}

namespace divil
{
    void Serialize(Serializer& ser, ManualWave& datum)
    {
        assert(ser.IsValid());
        Serialize(ser, datum.data.amount);
        Serialize(ser, datum.data.kind);
        Serialize(ser, datum.overide);
        Serialize(ser, datum.Time);
        Serialize(ser, datum.wave);
    }
}

void EnemyWaveSystem::Init()
{
    gWaveInspector = new EnemyWaveInspector();
    gWaveInspector->AddEnemyType("Default", Kind_Enemy);
    gWaveInspector->AddEnemyType("Transporter", Kind_EnemyTransporter);
    gWaveInspector->AddEnemyType("Kamikaze", Kind_EnemyKamikaze);
    gWaveInspector->AddEnemyType("Sniper", Kind_EnemySniper);
    gWaveInspector->AddEnemyType("tank", Kind_EnemyTank);
    //TODO points should actually be serialised

    gDifficultyCurve = new DifficultyCurve();

    divil::Serializer ser;
    SerializeEnemyWaves(divil::SerializeMode_Read);
    if (gDifficultyCurve->GetBezierSize() == 0)
    {
        gDifficultyCurve->AddBezierPoint({ 0,0 });
        gDifficultyCurve->AddBezierPoint({ 100,100 });
    }
}