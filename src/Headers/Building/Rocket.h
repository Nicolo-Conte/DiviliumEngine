#pragma once
#include "Turret.h"
class Rocket :
    public Turret
{
private:

public:
    Rocket();
    Rocket(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
    ~Rocket();
     void AttackFunction() override;
};

