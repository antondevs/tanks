//
//  Engine.hpp
//  Tanks
//
//  Created by antondev on 06/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#ifndef __TANKS_ENGINE_H
#define __TANKS_ENGINE_H

#include <vector>
#include <SDL_audio.h>
#include <SDL_main.h>
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Settings.h"

struct Sound
{
    Mix_Chunk *pChunk;
    char szName[32];
};

class Engine
{
private:
    bool m_bActive;
    SDL_Window* m_pWindow;
    SDL_GLContext m_pContext;
    SDL_Event m_oEvent;
    std::vector<Sound> m_lSounds;
    
protected:
    virtual bool OnStartup();
    virtual void OnShutdown();
    virtual void OnRender();
    virtual void OnKeyDown(int nKey);
    virtual void OnKeyUp(int nKey);
    bool LoadFile(const char *pszFileName, char **pData, int &nSize);
    void PlaySound(const char *pszName);
    void LoadSound(const char *pszFileName);
    
    
public:
    Engine();
    ~Engine();
    
    bool Init();
    void Start();
};

#endif /* Engine_hpp */
