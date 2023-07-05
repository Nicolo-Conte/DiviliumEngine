#pragma once
#include "Turret.h"
class Machinegun :
    public Turret
{
private:

public:
    Machinegun();
    Machinegun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
    ~Machinegun();
};

