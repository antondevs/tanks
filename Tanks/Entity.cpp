//
//  Entity.cpp
//  Tanks
//
//  Created by antondev on 07/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#include "Entity.h"

Entity::Entity()
{
    m_pTexture = NULL;
    m_fSize = 0.2f;;
    m_fAngle = 0.0f;
    m_fX = m_fY = 0.0f;
    m_nSkin = 0;
    m_fSpeed = 0.5f;
    m_fHealt = 100.0f;
    m_fGuard = 0.5f;
    m_fDamageLevel = 0.0f;
}

Entity::Entity(Texture *pTexture, float fSize)
{
    m_pTexture = pTexture;
    m_fSize = fSize;
    m_fAngle = 0.0f;
    m_fX = m_fY = 0.0f;
    m_nSkin = 0;
    m_fSpeed = 0.5f;
    m_fWaveAngle = 0.0f;
    m_fHealt = 100.0f;
    m_fGuard = 0.5f;
    m_fDamageLevel = 0.0f;
}

Entity::~Entity()
{
    
}

float Entity::GetDamageLevel()
{
    return m_fDamageLevel;
}

void Entity::SetDamageLevel(float fValue)
{
    m_fDamageLevel = fValue;
}

float Entity::GetHealt()
{
    return m_fHealt;
}

float Entity::GetGuard()
{
    return m_fGuard;
}

void Entity::SetGuard(float fValue)
{
    m_fGuard = fValue;
}

void Entity::SetWaveAngle(float fAngle)
{
    m_fWaveAngle = fAngle;
}

void Entity::SetSize(float fValue)
{
    m_fSize = fValue;
}

float Entity::GetSize()
{
    if (m_pTexture)
        return m_fSize * m_pTexture->fRatio > m_fSize ? m_fSize * m_pTexture->fRatio : m_fSize;
        
    return m_fSize;
}

float Entity::GetNextX(float fDelay, int nDirection)
{
    float fVal = fDelay * m_fSpeed;
    return m_fX + nDirection * fVal * sin(m_fAngle / 180.0f * 3.14f);
}

float Entity::GetNextY(float fDelay, int nDirection)
{
    float fVal = fDelay * m_fSpeed;
    return m_fY + nDirection * fVal * cos(m_fAngle / 180.0f * 3.14f);
}

void Entity::SetSpeed(float fValue)
{
    m_fSpeed = fValue;
}

float Entity::GetSpeed()
{
    return m_fSpeed;
}

void Entity::SetTexture(Texture *pTexture)
{
    m_pTexture = pTexture;
}

int Entity::GetSkin()
{
    return m_nSkin;
}

void Entity::SetSkin(int nSkin)
{
    m_nSkin = nSkin;
}

float Entity::GetAngle()
{
    return m_fAngle;
}

void Entity::SetAngle(float fValue)
{
    m_fAngle = fValue;
}

void Entity::SetX(float fValue)
{
    m_fX = fValue;
}

float Entity::GetX()
{
    return m_fX;
}

void Entity::SetY(float fValue)
{
    m_fY = fValue;
}

float Entity::GetY()
{
    return m_fY;
}

void Entity::SetHealt(float fHealt)
{
    m_fHealt = fHealt;
}

void Entity::Damage(float fValue)
{
    m_fHealt-=fValue;
    
    if (m_fHealt < 0)
        m_fHealt = 0.0f;        
}

void Entity::Draw()
{
    if (m_pTexture)
    {
        glPushMatrix();
        
        glTranslatef(m_fX, m_fY, 0.0f);
        glRotatef(180 - m_fAngle + m_fWaveAngle, 0.0f, 0.0f, 1.0f);
        
        glBegin(GL_QUADS);
        glTexCoord2f(m_pTexture->fU, m_pTexture->fV + m_pTexture->fH);
        glVertex2f(-m_fSize * m_pTexture->fRatio, -m_fSize);
        glTexCoord2f(m_pTexture->fU + m_pTexture->fW, m_pTexture->fV + m_pTexture->fH);
        glVertex2f( m_fSize * m_pTexture->fRatio, -m_fSize);
        glTexCoord2f(m_pTexture->fU + m_pTexture->fW, m_pTexture->fV);
        glVertex2f( m_fSize * m_pTexture->fRatio,  m_fSize);
        glTexCoord2f(m_pTexture->fU, m_pTexture->fV);
        glVertex2f(-m_fSize * m_pTexture->fRatio,  m_fSize);
        glEnd();
        
        glPopMatrix();
    }
    
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glTranslatef(m_fX, m_fY - m_fSize / 3, 0.0f);
    
    glBegin(GL_QUADS);
    
    float fProgress = (100.0f - m_fHealt) / 100.0f;
    
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f, -LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f( LIVE_INDICATOR_SIZE / 2.0f, -LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f( LIVE_INDICATOR_SIZE / 2.0f,  LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f,  LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    
    glColor4f(0.8f, 0.1f, 0.1f, 0.6f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f, -LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f + LIVE_INDICATOR_SIZE * fProgress, -LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f + LIVE_INDICATOR_SIZE * fProgress,  LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    glVertex2f(-LIVE_INDICATOR_SIZE / 2.0f,  LIVE_INDICATOR_SIZE / 2.0f * 0.2f);
    
    glEnd();
    
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
