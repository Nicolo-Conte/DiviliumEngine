#pragma once
#include "DebugUI/Inspectable.h"

class EntitySpawner:
	public Inspectable
{
	const static int MaxSize = 10;

	const char* UIlist[MaxSize];
	Kind KindList[MaxSize];

	int currentItem = 0;
	int ListSize = 0;
	bool mActive;
	int SpawnAmount = 1;
	glm::vec2 MousePos();
public:
	EntitySpawner();
	void Display() override;
	void AddItem(const char* aName,Kind aKind);
	void Update();
	void Spawn(int index,glm::vec2 aPlace);
};