#include "building/building.h"

Building::Building()
{
	mCost = 0;
	mHealth = 0;
	mHover = false;
    mCollisionLayer = Collision::CreateCollisionLayerWithName("Buildings");
}


void Building::DoDamage(int aDamageValue)
{
    mHealth -= aDamageValue;
    if (mHealth <= 0)
    {
        DeathFunction();
    }
}


/// <summary>
/// functionality of all turrets when you hover the cursor over them
/// </summary>
void Building::HealingFunction()
{

    mHealth += mHealingValue;
    if (mHealth > mMaxHealth)
    {
        mHealth = mMaxHealth;
    }
   
}

BuildingInspectable::BuildingInspectable()
{
    SetName("Edit Buildings");


	LoadSave(divil::SerializeMode_Read);

}

void BuildingInspectable::Display()
{
	if (ImGui::CollapsingHeader("Machine Gun"))
	{
		ImGui::InputFloat("Health", &HEALTHMACHINEGUN);
		ImGui::InputInt("Cost", &COSTMACHINEGUN);
		ImGui::InputInt("Attack", &ATTACKMACHINEGUN);
		ImGui::InputInt("Range", &RANGEMACHINEGUN);
		ImGui::InputFloat("Fire Rate", &HEALTHMACHINEGUN);
	}

	if (ImGui::CollapsingHeader("Shotgun"))
	{
		ImGui::InputFloat("Health", &HEALTHSHOTGUN);
		ImGui::InputInt("Cost", &COSTSHOTGUN);
		ImGui::InputInt("Attack", &ATTACKSHOTGUN);
		ImGui::InputInt("Range", &RANGESHOTGUN);
		ImGui::InputFloat("Fire Rate", &FIRERATESHOTGUN);
	}

	if (ImGui::CollapsingHeader("Rocket"))
	{
		ImGui::InputFloat("Health", &HEALTHROCKET);
		ImGui::InputInt("Cost", &COSTROCKET);
		ImGui::InputInt("Attack", &ATTACKROCKET);
		ImGui::InputInt("Range", &RANGEROCKET);
		ImGui::InputFloat("Fire Rate", &FIRERATEROCKET);
		ImGui::InputFloat("Explosion Radius", &ROCKETEXPLOSIONRADIUS);
	}

	if (ImGui::Button("Save"))
	{
		LoadSave(divil::SerializeMode_Write);
	}
}

void BuildingInspectable::LoadSave(divil::SerializeMode mode)
{
	divil::Serializer ser;
	if (BeginSerializeFile(ser, "Buildings.data", 0, mode))
	{
		Serialize(ser, HEALTHMACHINEGUN);
		Serialize(ser, COSTMACHINEGUN);
		Serialize(ser, ATTACKMACHINEGUN);
		Serialize(ser, RANGEMACHINEGUN);
		Serialize(ser, FIRERATEMACHINEGUN);

		Serialize(ser, HEALTHMACHINEGUN);
		Serialize(ser, COSTMACHINEGUN);
		Serialize(ser, ATTACKMACHINEGUN);
		Serialize(ser, RANGEMACHINEGUN);
		Serialize(ser, FIRERATEMACHINEGUN);

		Serialize(ser, HEALTHROCKET);
		Serialize(ser, COSTROCKET);
		Serialize(ser, ATTACKROCKET);
		Serialize(ser, RANGEROCKET);
		Serialize(ser, FIRERATEROCKET);
		Serialize(ser, ROCKETEXPLOSIONRADIUS);

		EndSerializeFile(ser);
	}
}



BuildingInspectable* BuildingInspectable::m_instance = nullptr;