#pragma once
#include "Bullet.h"
class BulletRocket :
    public Bullet
{
private:

    float mExplosionRadius;


public:

    BulletRocket();
    ~BulletRocket();
    void Attack(Entity* aEntity) override;
};

