#pragma once
#include "Turret.h"
class Shotgun :
    public Turret
{
private:

public:
    Shotgun();
    Shotgun(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
    ~Shotgun();
    void AttackFunction() override;
};

