#include "Entities/Collision.h"
#include "Entities/Entity.h"
#include "DebugUI/Inspectable.h"
#include "Serialize/Serialize.h"

using namespace divil;

static int mNextAvailableLayer;
static constexpr int MAX_LAYERS = 16;
static divil::InlineArray<std::string, MAX_LAYERS> mLayers;
static divil::SmallMap<std::string, int> mLayerMap;
static bool mCollisionMatrix[MAX_LAYERS][MAX_LAYERS];
static bool mCondition;

float gSaveTimer = 0;
class CollisionInspector :
    public Inspectable
{
public:
    CollisionInspector()
    {
        SetName("CollisionData");
    }
    void Display()
    {
        // Display text field explaining how it works
        ImGui::Text("You can simply change a layer's \ncollision with another layer and save it ");
        // I guess for applying new names if needed
        //static char layerName[30] = "";
        //IMGUI_LEFT_LABEL(ImGui::InputText, "New Name: ", layerName, IM_ARRAYSIZE(layerName));

        // Showcase all layers with boolean to check on and of
        for (int i = 0; i < mLayers.Count(); i++)
        {
            std::string mainLayer("");
            mainLayer.append(std::to_string(i)).append(" : ").append(mLayers[i]);

            if (ImGui::CollapsingHeader(mainLayer.c_str()))
            {
                // I guess for applying new names if needed
                //if (ImGui::Button("Apply New Name"))
                //{
                //    mLayers[i] = layerName;
                //}

                for (int j = 0; j < mLayers.Count(); j++)
                {
                    std::string secondLayer("");
                    secondLayer.append(mLayers[j]);
                    std::string layerID("");
                    layerID = mainLayer + secondLayer;
                    ImGui::PushID(layerID.c_str());
                    ImGui::Checkbox(secondLayer.c_str(), &mCollisionMatrix[i][j]);
                    ImGui::PopID();
                    ImGui::Spacing();
                }
            }
        }

        // Display button to save layer settings
        if ( ImGui::Button("Save Settings"))
        {
            Serializer ser;
            if (BeginSerializeFile(ser, "collision.data", 0, divil::SerializeMode_Write))
            {
                gSaveTimer = 3;
                Serialize(ser, mLayers);
                Serialize(ser, mNextAvailableLayer);
                Serialize(ser, mLayerMap);
                Serialize(ser, mCollisionMatrix);
                EndSerializeFile(ser);
            }
        }

        if (gSaveTimer > 0)
        {
            ImGui::Text("Saved!");
            gSaveTimer -= 1 / 60.0f;
        }
    }
};
CollisionInspector* gCollisionInspector;

Collision::Collision()
{
    mNextAvailableLayer = 0;
}

Collision::~Collision()
{  }

// Init the collision data serializer call this before the update loop starts
void Collision::Init()
{
    gCollisionInspector = new CollisionInspector();
    mCollisionMatrix[0][0] = true;
    mLayers.Add("Default");
    mLayerMap.Add("Default", 0);
    mNextAvailableLayer++;

    divil::Serializer ser;
    if (BeginSerializeFile(ser, "collision.data", 0, divil::SerializeMode_Read))
    {
        Serialize(ser, mLayers);
        Serialize(ser, mNextAvailableLayer);
        Serialize(ser, mLayerMap);
        Serialize(ser, mCollisionMatrix);
        EndSerializeFile(ser);
    }
}

// AABB Entity A vs Entity B Collision Check
bool Collision::EntityVsEntity(Entity* a, Entity* b)
{
    if (a->mTransform.mPosition.x < b->mTransform.mPosition.x + b->mTransform.mSize.x * b->mTransform.mScale.x &&
        a->mTransform.mPosition.x + a->mTransform.mSize.x * a->mTransform.mScale.x > b->mTransform.mPosition.x &&
        a->mTransform.mPosition.y < b->mTransform.mPosition.y + b->mTransform.mSize.y * b->mTransform.mScale.y &&
        a->mTransform.mPosition.y + a->mTransform.mSize.y * a->mTransform.mScale.y > b->mTransform.mPosition.y )
    {
        return true;
    }

    return false;
}

// AABB Dummy A vs Dummy B Collision Check
bool Collision::EntityVsEntity(Collider a, Collider b)
{
    if (a.mPosition.x < b.mPosition.x + b.mSize.x * b.mScale.x &&
        a.mPosition.x + a.mSize.x * a.mScale.x > b.mPosition.x &&
        a.mPosition.y < b.mPosition.y + b.mSize.y * b.mScale.y &&
        a.mPosition.y + a.mSize.y * a.mScale.y > b.mPosition.y)
    {
        return true;
    }

    return false;
}

// AABB Point(mousePos) vs Entity Collision Check
bool Collision::PointVsEntity(glm::vec2 mousePos, Entity* b)
{
    if (mousePos.x >= b->mTransform.mPosition.x &&
        mousePos.y >= b->mTransform.mPosition.y &&
        mousePos.x < b->mTransform.mPosition.x + b->mTransform.mSize.x * b->mTransform.mScale.x &&
        mousePos.y < b->mTransform.mPosition.y + b->mTransform.mSize.y * b->mTransform.mScale.y)
    {
        return true;
    }

    return false;
}

// Entity A vs Entity B Circle Collision Check
bool Collision::CircleCollision(Entity* a, Entity* b)
{
    float distX = a->mTransform.mPosition.x + a->mCollider.mOffset.x - b->mTransform.mPosition.x + b->mCollider.mOffset.x;
    float distY = a->mTransform.mPosition.y + a->mCollider.mOffset.x - b->mTransform.mPosition.y + + b->mCollider.mOffset.y;
    float radiSum = a->mCollider.mRadius + b->mCollider.mRadius;

    return distX * distX + distY * distY <= radiSum * radiSum;
}

// Dummy A vs Dummy B Circle Collision Check
bool Collision::CircleCollision(Collider a , Collider b)
{
    float distX = a.mPosition.x + a.mOffset.x - b.mPosition.x + b.mOffset.x;
    float distY = a.mPosition.y + a.mOffset.x - b.mPosition.y + +b.mOffset.y;
    float radiSum = a.mRadius + b.mRadius;

    return distX * distX + distY * distY <= radiSum * radiSum;
}

// Entity A vs Entity B Collision Layer Check
bool Collision::CollisionLayer(int a, int b)
{
    //   0 1 2 3 4 5 6 7
    // 0 x
    // 1     x
    // 2   x
    // 3
    // 4             x
    // 5
    // 6         x
    // 7

    if (a > b)
    {
        Swap(a, b);
    }

    return mCollisionMatrix[a][b];
}

// Set the collision condition between layer x and y
void Collision::SetCollisionLayerCondition(int aLayerA, int aLayerB, bool aCondition)
{
    if (aLayerA > aLayerB)
    {
        Swap(aLayerA, aLayerB);
    }

    mCollisionMatrix[aLayerA][aLayerB] = aCondition;
}

// you kinda dont want to do this often so, it's an annoyingly long name
int Collision::CreateCollisionLayerWithName(const std::string &aName)
{
    bool contains = false;

    if (mNextAvailableLayer < MAX_LAYERS)
    {
        for (auto& layer : mLayers)
        {
            if (layer == aName)
            {
                contains = true;
            }
        }

        if (!contains)
        {

            mLayers.Add(aName);
            mLayerMap.Add(aName, mNextAvailableLayer);
            for (int i = 0; i < mLayers.Count(); i++)
            {
                for (int j = 0; j < mLayers.Count(); j++)
                {
                    mCollisionMatrix[i][j] = true;
                }
            }
            mNextAvailableLayer++;
            return mNextAvailableLayer;
        }
        else
        {
            return GetCollisionLayerWithName(aName);
        }
    }
    else
    {
        assert("Looks like someone is trying to create more layers than you're allowed to bad person!");
    }

    return -1;
}

// you dont want to do this often so, it's an annoyingly long name
int Collision::GetCollisionLayerWithName(const std::string& aName)
{
    int result = -1;
    mLayerMap.Find(aName, &result);
    return result;
}