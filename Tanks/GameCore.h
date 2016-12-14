//
//  GameCore.hpp
//  Tanks
//
//  Created by antondev on 06/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#ifndef __TANKS_GAMECORE_H
#define __TANKS_GAMECORE_H

#include <math.h>
#include <vector>
#include <SDL_image.h>
#include "Engine.h"
#include "cJSON.h"
#include "Texture.h"
#include "Entity.h"
#include "Bullet.h"

class GameCore : public Engine
{
private:
    bool m_bGameOver;
    GLuint m_nAtlas;
    long m_nLastTime;
    float m_fFrameDelay;
    float m_fWaveRotate;
    std::vector<Texture *> m_lTextures;
    
    long m_nBulletTime;
    Bullet m_lBullets[BULLETS_COUNT];
    
    Entity *m_pPlayer;
    Texture *m_pBackground;
    Texture *m_pBullet;
    Texture *m_pEndGame;
    
    std::vector<Entity *> m_lMonsters;
    
    bool LoadAtlasList();
    Texture *FindTexture(const char *pszName);
    void HandleControl();
    long GetTimeMilliseconds();
    
    void ChangeWeapon(int nIndex);
    
    float GetNextX(float fValue, float fX);
    float GetNextY(float fValue, float fY);
    Entity *DetectCollision(float fX, float fY, Entity *pIgnore = NULL);
    
    void InitMonsters();
    void SetRandomMonster(int nIndex, Entity *pMonster);
    void MoveMonster(Entity *pMonster);
    void MonsterDamage(Entity *pMonster, int nWeapon);
    
    float GetBulletSpeed();
    
    void Restart();
    
protected:
    bool OnStartup();
    void OnShutdown();
    void OnRender();
    void OnKeyDown(int nKey);
    void OnKeyUp(int nKey);
    
    
    
public:
    GameCore();
    ~GameCore();
};

#endif /* GameCore_hpp */
