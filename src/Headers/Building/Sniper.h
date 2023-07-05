#pragma once
#include "Turret.h"
class Sniper :
    public Turret
{
private:

public:
    Sniper();
    Sniper(int aAttackRange, int aAttackValue, float aFireRate, float aHealth);
    ~Sniper();
    void DetectionRange() override;
};

