//
//  Bullet.h
//  Tanks
//
//  Created by antondev on 07/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#ifndef __TANKS_BULLET_H
#define __TANKS_BULLET_H

#include "Entity.h"

struct Bullet
{
    bool bActive;
    float fAngle;
    float fX;
    float fY;
    int nWeapon;
    float fSpeed;
    
    Bullet()
    {
        nWeapon = 0;
        fSpeed = 40;
        fX = 0.0f;
        fY = 0.0f;
        bActive = false;
        fAngle = 0.0f;
    }
};

#endif /* Bullet_h */
