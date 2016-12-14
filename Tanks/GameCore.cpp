//
//  GameCore.cpp
//  Tanks
//
//  Created by antondev on 06/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#include "GameCore.h"

GameCore::GameCore() : Engine(/* TODO ARGS */)
{
    m_pBackground = NULL;
}

GameCore::~GameCore()
{
    for (int i = 0; i < m_lTextures.size(); i++)
        delete (m_lTextures.at(i));
    
    m_lTextures.clear();
}

void GameCore::SetRandomMonster(int nIndex, Entity *pMonster)
{
    if (pMonster)
    {
        int nSkin = rand() % 3;
        
        if (nSkin == 0)
            pMonster->SetTexture(FindTexture("monster1.png"));
        
        if (nSkin == 1)
            pMonster->SetTexture(FindTexture("monster2.png"));
        
        if (nSkin == 2)
            pMonster->SetTexture(FindTexture("monster3.png"));
        
        pMonster->SetSize(0.12f);
        
        float fX = 0;
        float fY = 0;
        
        do {
            int nX = rand() % 100;
            int nY = rand() % 100;
            fX = ((nX - 50) / 50.0f);
            fY = ((nY - 50) / 50.0f);
            
            fX = fX > 0 ? (1.10f + fX) : (fX - 1.10f);
            
        } while (DetectCollision(fX, fY, pMonster));
        
        pMonster->SetX(fX);
        pMonster->SetY(fY);
        pMonster->SetHealt(100.0f);
        
        int nValue = rand() % 100;
        pMonster->SetSpeed(nValue / 2500.0f + 0.003);
        
        nValue = rand() % 100;
        pMonster->SetGuard(nValue / 100.0f);
        
        nValue = rand() % 100;
        pMonster->SetDamageLevel(nValue / 100.0f);
    }
}

void GameCore::InitMonsters()
{
    srand((unsigned int) time(NULL));
    
    for (int i = 0; i < MONSTER_COUNT; i++)
        m_lMonsters.push_back(new Entity());
}

bool GameCore::LoadAtlasList()
{
    int nSize = 0;
    char *pData = NULL;
    
    if (!LoadFile("atlas.json", &pData, nSize))
        return false;
    
    cJSON *json = cJSON_Parse(pData);
    
    if (!json)
        return false;
    
    cJSON *frames = cJSON_GetObjectItem(json, "frames");
    cJSON *meta = cJSON_GetObjectItem(json, "meta");
    
    if (frames && meta)
    {
        cJSON *size = cJSON_GetObjectItem(meta, "size");
        float width = cJSON_GetObjectItem(size, "w")->valueint;
        float height = cJSON_GetObjectItem(size, "h")->valueint;
        
        int nCount = cJSON_GetArraySize(frames);
        
        for (int i = 0; i < nCount; i++)
        {
            cJSON *frame = cJSON_GetArrayItem(frames, i);
            cJSON *frameInfo = cJSON_GetObjectItem(frame, "frame");
            
            Texture *pTexture = new Texture();
            
            strcpy(pTexture->szName, cJSON_GetObjectItem(frame, "filename")->valuestring);
            pTexture->fU = cJSON_GetObjectItem(frameInfo, "x")->valueint / width;
            pTexture->fV =  cJSON_GetObjectItem(frameInfo, "y")->valueint / height;
            pTexture->fW = cJSON_GetObjectItem(frameInfo, "w")->valueint / width;
            pTexture->fH = cJSON_GetObjectItem(frameInfo, "h")->valueint / height;
            pTexture->fRatio = cJSON_GetObjectItem(frameInfo, "w")->valueint / (float) cJSON_GetObjectItem(frameInfo, "h")->valueint;
            
            m_lTextures.push_back(pTexture);
        }
    }
    
    cJSON_Delete(json);
    free(pData);
    
    return true;
}

Texture *GameCore::FindTexture(const char *pszName)
{
    for (int i = 0; i < m_lTextures.size(); i++)
    {
        if (strcmp(pszName, m_lTextures[i]->szName) == 0)
            return m_lTextures[i];
    }
    
    return NULL;
}

bool GameCore::OnStartup()
{
    int imgFlags = IMG_INIT_PNG;
    
    if( !( IMG_Init( imgFlags ) & imgFlags ) || !LoadAtlasList() )
        return false;
    
    SDL_Surface *pAtlas = IMG_Load("atlas.png");
    
    if (!pAtlas)
        return false;
    
    if (pAtlas->format->BytesPerPixel != 4)
    {
        SDL_FreeSurface(pAtlas);
        return false;
    }
    
    glGenTextures(1,&m_nAtlas);
    glBindTexture(GL_TEXTURE_2D, m_nAtlas);
    
    
    // Invert color channels
    
    int nPixCount = pAtlas->w * pAtlas->h;
    unsigned char *pBuffer = (unsigned char *) malloc (nPixCount * 4);
    memcpy(pBuffer, pAtlas->pixels, nPixCount * 4);
    
    unsigned char *pPointer = pBuffer;
    
    for (int i = 0; i < nPixCount; i++)
    {
        unsigned char temp = pPointer[0];
        pPointer[0] = pPointer[2];
        pPointer[2] = temp;
        pPointer+=4;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, pAtlas->format->BytesPerPixel, pAtlas->w, pAtlas->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
    SDL_FreeSurface(pAtlas);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(pBuffer);
        
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_pEndGame = FindTexture("gameover.png");
    m_pBullet = FindTexture("bullet.png");
    m_pBackground = FindTexture("background.png");
    m_pPlayer = new Entity(FindTexture("player_blue.png"), 0.2f);
    
    m_nLastTime = GetTimeMilliseconds();
    m_nBulletTime = m_nLastTime;
    m_fFrameDelay = 0.0f;
    
    InitMonsters();
    
    m_fWaveRotate = 0.0f;
    
    LoadSound("shoot.wav");
    LoadSound("gameover.wav");
    LoadSound("dead.wav");
    
    Restart();
    
    return true;
}

void GameCore::OnShutdown()
{
    
}


long GameCore::GetTimeMilliseconds()
{
    return SDL_GetTicks();
}

void GameCore::MonsterDamage(Entity *pMonster, int nWeapon)
{
    if (pMonster)
    {
        float fDamage = nWeapon == 0 ? WEAPON_LEVEL_1 : (nWeapon == 1 ? WEAPON_LEVEL_2 : WEAPON_LEVEL_3);
        fDamage = fDamage * pMonster->GetGuard();
        pMonster->Damage(fDamage);
        
        if (pMonster->GetHealt() == 0.0f)
        {
            SetRandomMonster(-1, pMonster);
            PlaySound("dead.wav");
        }
    }
}

void GameCore::OnRender()
{
    glClearColor(0.15f, 0.65f, 0.35f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    long nTime = GetTimeMilliseconds();
    m_fFrameDelay = (nTime - m_nLastTime) / 1000.0f;
    m_nLastTime = nTime;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_nAtlas);
    
    if (m_pBackground)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(m_pBackground->fU, m_pBackground->fV + m_pBackground->fH);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(m_pBackground->fU + m_pBackground->fW, m_pBackground->fV + m_pBackground->fH);
        glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(m_pBackground->fU + m_pBackground->fW, m_pBackground->fV);
        glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(m_pBackground->fU, m_pBackground->fV);
        glVertex2f(-1.0f,  1.0f);
        glEnd();
    }
    
    for (int i = 0; i < BULLETS_COUNT; i++)
    {
        if (m_lBullets[i].bActive && m_pBullet)
        {
            glPushMatrix();
            
            glTranslatef(m_lBullets[i].fX, m_lBullets[i].fY, 0.0f);
            glRotatef(180 - m_lBullets[i].fAngle, 0.0f, 0.0f, 1.0f);
            
            glBegin(GL_QUADS);
            glTexCoord2f(m_pBullet->fU, m_pBullet->fV + m_pBullet->fH);
            glVertex2f(-BULLET_SIZE * m_pBullet->fRatio, -BULLET_SIZE);
            glTexCoord2f(m_pBullet->fU + m_pBullet->fW, m_pBullet->fV + m_pBullet->fH);
            glVertex2f( BULLET_SIZE * m_pBullet->fRatio, -BULLET_SIZE);
            glTexCoord2f(m_pBullet->fU + m_pBullet->fW, m_pBullet->fV);
            glVertex2f( BULLET_SIZE * m_pBullet->fRatio,  BULLET_SIZE);
            glTexCoord2f(m_pBullet->fU, m_pBullet->fV);
            glVertex2f(-BULLET_SIZE * m_pBullet->fRatio,  BULLET_SIZE);
            glEnd();
            
            glPopMatrix();
            
            float fVal = m_fFrameDelay * m_lBullets[i].fSpeed;
            float fX  = m_lBullets[i].fX + fVal * sin(m_lBullets[i].fAngle / 180.0f * 3.14f);
            float fY = m_lBullets[i].fY + fVal * cos(m_lBullets[i].fAngle / 180.0f * 3.14f);
            
            m_lBullets[i].fX = fX;
            m_lBullets[i].fY = fY;
            
            Entity *pEntity = DetectCollision(fX, fY, m_pPlayer);
            
            if (pEntity)
            {
                MonsterDamage(pEntity, m_lBullets[i].nWeapon);
                m_lBullets[i].bActive = false;
            }
        }
    }
    
    m_pPlayer->Draw();
    
    for (int i = 0; i < MONSTER_COUNT; i++)
    {
        Entity *pMonster = m_lMonsters[i];
        
        double fAngle = atan2(m_pPlayer->GetY() - pMonster->GetY(), m_pPlayer->GetX() - pMonster->GetX()) / 3.14f * 180.0f;
        pMonster->SetAngle(270.0f - fAngle);
        
        pMonster->SetWaveAngle(5 * sin(m_fWaveRotate / 20.0f * 3.14f));
        
        pMonster->Draw();
        
        MoveMonster(pMonster);
    }
    
    
    
    if (m_fFrameDelay < 1.0f)
    {
        HandleControl();
    }
    
    m_fWaveRotate+= 1;
    
    if ((GetTimeMilliseconds() - m_nBulletTime) / 1000.0f > BULLET_INTERVAL)
    {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        
        if (state[SDL_SCANCODE_X])
        {
            m_nBulletTime = GetTimeMilliseconds();
            
            for (int i = 0; i < BULLETS_COUNT; i++)
            {
                if (!m_lBullets[i].bActive)
                {
                    m_lBullets[i].fSpeed = GetBulletSpeed();
                    m_lBullets[i].fY = m_pPlayer->GetY();
                    m_lBullets[i].fX = m_pPlayer->GetX();
                    m_lBullets[i].fAngle = m_pPlayer->GetAngle();
                    PlaySound("shoot.wav");
                    m_lBullets[i].bActive = true;
                    
                    break;
                }
            }
        }
    }
    
    if (m_bGameOver && m_pEndGame)
    {
        glBegin(GL_QUADS);
        glTexCoord2f(m_pEndGame->fU, m_pEndGame->fV + m_pEndGame->fH);
        glVertex2f(-0.3f, -0.15f);
        glTexCoord2f(m_pEndGame->fU + m_pEndGame->fW, m_pEndGame->fV + m_pEndGame->fH);
        glVertex2f( 0.3f, -0.15f);
        glTexCoord2f(m_pEndGame->fU + m_pEndGame->fW, m_pEndGame->fV);
        glVertex2f( 0.3f,  0.15f);
        glTexCoord2f(m_pEndGame->fU, m_pEndGame->fV);
        glVertex2f(-0.3f,  0.15f);
        glEnd();
    }
    
    glDisable(GL_TEXTURE_2D);
}

void GameCore::Restart()
{
    m_pPlayer->SetHealt(100.0f);
    m_pPlayer->SetX(0);
    m_pPlayer->SetY(0);
    m_pPlayer->SetAngle(0.0f);
    
    int nValue = rand() % 100;
    m_pPlayer->SetGuard(nValue / 100.0f);
    
    for (int i = 0; i < MONSTER_COUNT; i++)
    {
        SetRandomMonster(i, m_lMonsters.at(i));
    }
    
    m_bGameOver = false;
}

float GameCore::GetBulletSpeed()
{
    if (m_pPlayer->GetSkin() == 0) {
        return 5;
    }
    else if (m_pPlayer->GetSkin() == 1) {
        return 10;
    }
    else if (m_pPlayer->GetSkin() == 2) {
        return 15;
    }
    
    return 0.02;
}

void GameCore::MoveMonster(Entity *pMonster)
{
    if (m_bGameOver)
        return;
    
    if (pMonster)
    {
        float fX = pMonster->GetNextX(m_fFrameDelay, -1);
        float fY = pMonster->GetNextY(m_fFrameDelay, -1);
        
        Entity *pObject = DetectCollision(fX, pMonster->GetY(), pMonster);
        
        if (pObject == NULL)
           pMonster->SetX(fX);
        else if (m_pPlayer != pObject && DetectCollision(fX + 0.01, pMonster->GetY(), pMonster) == NULL)
            pMonster->SetX(fX + 0.01);
        
        if (DetectCollision(pMonster->GetX(), fY, pMonster) == NULL)
                pMonster->SetY(fY);
        
        if (pObject == m_pPlayer)
        {
            float fDamage = pMonster->GetDamageLevel() * m_pPlayer->GetGuard();
            m_pPlayer->Damage(fDamage);
            
            if (m_pPlayer->GetHealt() <= 0.0f)
            {
                m_bGameOver = true;
                PlaySound("gameover.wav");
                return;
            }
        }
    }
}

Entity *GameCore::DetectCollision(float fX, float fY, Entity *pIgnore)
{
    float fDistance = 0.0f;
    
    for (int i = 0; i < MONSTER_COUNT; i++)
    {
        Entity *pMonster  = m_lMonsters.at(i);
        
        if (pIgnore == pMonster)
            continue;
        
        fDistance = COLLISION_SPACE * sqrt(pow(fX - pMonster->GetX(), 2) + pow(fY - pMonster->GetY(), 2));
        
        if (fDistance < (pMonster->GetSize() / 2) + (pIgnore->GetSize() / 2))
            return pMonster;
    }
    
    if (pIgnore != m_pPlayer)
    {
        fDistance = COLLISION_SPACE * sqrt(pow(fX - m_pPlayer->GetX(), 2) + pow(fY - m_pPlayer->GetY(), 2));
        
        if (fDistance < m_pPlayer->GetSize() / 2 + (pIgnore->GetSize() / 2))
            return m_pPlayer;
    }
    
    return NULL;
}

void GameCore::HandleControl()
{
    if (m_bGameOver)
        return;
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_RIGHT])
    {
        float fAngle = m_pPlayer->GetAngle();
        fAngle+= PLAYER_ROTATE_SPEED * m_fFrameDelay;
        m_pPlayer->SetAngle(fAngle);
    }
    
    if (state[SDL_SCANCODE_LEFT])
    {
        float fAngle = m_pPlayer->GetAngle();
        fAngle-= PLAYER_ROTATE_SPEED * m_fFrameDelay;
        m_pPlayer->SetAngle(fAngle);
    }
    
    if (state[SDL_SCANCODE_UP])
    {
        float fSize = m_pPlayer->GetSize();
        float fX = m_pPlayer->GetNextX(m_fFrameDelay, 1);
        float fY = m_pPlayer->GetNextY(m_fFrameDelay, 1);
        
        if (fX - fSize / 2 > -1.0 && fX + fSize / 2 < 1.0)
        {
            if (DetectCollision(fX, m_pPlayer->GetY(), m_pPlayer) == NULL)
                m_pPlayer->SetX(fX);
        }
        
        if (fY - fSize / 2 > -1.0 && fY + fSize / 2 < 1.0)
        {
            if (DetectCollision(m_pPlayer->GetX(), fY, m_pPlayer) == NULL)
                m_pPlayer->SetY(fY);
        }
    }
    
    if (state[SDL_SCANCODE_DOWN])
    {
        float fSize = m_pPlayer->GetSize();
        float fX = m_pPlayer->GetNextX(m_fFrameDelay, -1);
        float fY = m_pPlayer->GetNextY(m_fFrameDelay, -1);
        
        if (fX - fSize / 2 > -1.0 && fX + fSize / 2 < 1.0)
        {
            if (!DetectCollision(fX, m_pPlayer->GetY(), m_pPlayer))
            {
                m_pPlayer->SetX(fX);
            }
        }
        
        if (fY - fSize / 2 > -1.0 && fY + fSize / 2 < 1.0)
        {
            if (!DetectCollision(m_pPlayer->GetX(), fY, m_pPlayer))
            {
                m_pPlayer->SetY(fY);
            }
        }
    }
}

void GameCore::OnKeyDown(int nKey)
{
    
}

float GameCore::GetNextX(float fValue, float fX)
{
    return 0;
}

float GameCore::GetNextY(float fValue, float fY)
{
    return 0;
}

void GameCore::ChangeWeapon(int nIndex)
{
    if (nIndex < 0)
        nIndex = 2;
    
    if (nIndex > 2)
        nIndex = 0;
    
    if (nIndex >= 0 && nIndex < 3)
    {
        if (nIndex == 0)
        {
            m_pPlayer->SetTexture(FindTexture("player_blue.png"));
        }
        else if (nIndex == 1)
        {
            m_pPlayer->SetTexture(FindTexture("player_red.png"));
        }
        else if (nIndex == 2)
        {
            m_pPlayer->SetTexture(FindTexture("player_green.png"));
        }
        
        
        m_pPlayer->SetSkin(nIndex);
    }
}

void GameCore::OnKeyUp(int nKey)
{
    if (m_bGameOver)
    {
        Restart();
        return;
    }
    
    if (nKey == SDLK_w)
    {
        int nSkin = m_pPlayer->GetSkin();
        ChangeWeapon(nSkin+1);
    }
    
    if (nKey == SDLK_r)
    {
        Restart();
    }
    
    if (nKey == SDLK_q)
    {
        int nSkin = m_pPlayer->GetSkin();
        ChangeWeapon(nSkin-1);
    }
}
