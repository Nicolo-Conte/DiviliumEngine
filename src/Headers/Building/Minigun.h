#pragma once
#include "Turret.h"
class Minigun :
    public Turret
{
private:

public:
    Minigun();
    Minigun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
    ~Minigun();
};

