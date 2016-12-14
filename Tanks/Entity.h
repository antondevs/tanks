//
//  Entity.hpp
//  Tanks
//
//  Created by antondev on 07/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#ifndef __TANKS_ENTITY_H
#define __TANKS_ENTITY_H

#include <math.h>
#include "Settings.h"
#include <SDL_opengl.h>
#include "Texture.h"

class Entity
{
private:
    Texture *m_pTexture;
    float m_fSize;
    float m_fAngle;
    float m_fX;
    float m_fY;
    int m_nSkin;
    float m_fSpeed;
    float m_fWaveAngle;
    float m_fHealt;
    float m_fGuard;
    float m_fDamageLevel;
    
public:
    Entity(Texture *pTexture, float fSize);
    Entity();
    ~Entity();
    
    void Damage(float fValue);
    
    void SetHealt(float fHealt);
    
    void Draw();
    void SetWaveAngle(float fAngle);
    
    float GetAngle();
    void SetAngle(float fValue);
    
    int GetSkin();
    void SetSkin(int nSkin);
    void SetTexture(Texture *pTexture);
    
    void SetX(float fValue);
    float GetX();
    
    void SetY(float fValue);
    float GetY();
    
    void SetSpeed(float fValue);
    float GetSpeed();
    
    float GetNextX(float fDelay, int nDirection = 1);
    float GetNextY(float fDelay, int nDirection = 1);
    
    float GetSize();
    void SetSize(float fValue);
    
    float GetGuard();
    void SetGuard(float fValue);
    
    float GetDamageLevel();
    void SetDamageLevel(float fValue);
    
    float GetHealt();

};

#endif /* Entity_hpp */
