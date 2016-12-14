//
//  Engine.cpp
//  Tanks
//
//  Created by antondev on 06/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#include "Engine.h"

Engine::Engine()
{
    m_bActive = false;
    m_pWindow = NULL;
    m_pContext = NULL;
    
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
    }
    
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        printf("Unable to initialize audio: %s\n", Mix_GetError());
    }
}

Engine::~Engine()
{
    if (m_pContext)
        SDL_GL_DeleteContext(m_pContext);
    
    if (m_pWindow)
        SDL_DestroyWindow(m_pWindow);
    
    Mix_CloseAudio();
}

bool Engine::OnStartup()
{
    return false;
}

void Engine::OnShutdown()
{

}

void Engine::OnKeyDown(int nKey)
{
    
}

bool Engine::LoadFile(const char *pszFileName, char **pData, int &nSize)
{
    nSize = 0;
    *pData = NULL;
    
    FILE *fp = fopen(pszFileName, "rb");
    
    if (!fp)
        return false;
    
    fseek(fp, 0, SEEK_END);
    nSize = (int) ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    *pData = (char *) malloc (nSize);
    
    if (*pData == NULL)
    {
        fclose(fp);
        return false;
    }
    
    fread(*pData, nSize, 1, fp);
    fclose(fp);
    
    return true;
}

void Engine::LoadSound(const char *pszFileName)
{
    Sound oSounds;
    
    oSounds.pChunk = Mix_LoadWAV(pszFileName);
    
    if(oSounds.pChunk == NULL) {
        printf("Unable to load WAV file: %s\n", Mix_GetError());
        return;
    }
    
    strcpy(oSounds.szName, pszFileName);
    m_lSounds.push_back(oSounds);
}

void Engine::PlaySound(const char *pszName)
{
    for (int i = 0; i < m_lSounds.size(); i++)
    {
        if (strcmp(m_lSounds.at(i).szName, pszName) == 0)
        {
            Mix_PlayChannel(-1, m_lSounds.at(i).pChunk, 0);
        }
    }
}

void Engine::OnKeyUp(int nKey)
{
    
}

void Engine::OnRender()
{
    glClearColor(0.15f, 0.65f, 0.35f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::Start()
{
    if (!m_pContext || !m_pWindow)
        return;
    
    m_bActive = true;
    
    if (!OnStartup())
        return;
    
    while (m_bActive)
    {
        while(SDL_PollEvent(&m_oEvent) != 0)
        {
            if (m_oEvent.type == SDL_QUIT)
                m_bActive = false;
            
            if (m_oEvent.type == SDL_KEYDOWN)
                OnKeyDown(m_oEvent.key.keysym.sym);
            
            if (m_oEvent.type == SDL_KEYUP)
                OnKeyUp(m_oEvent.key.keysym.sym);
        }
        
        OnRender();
        
        SDL_GL_SwapWindow(m_pWindow);
    }
    
    OnShutdown();
}

bool Engine::Init()
{
    int nFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    
    m_pWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, nFlags );
    
    if (m_pWindow == NULL)
        return false;
    
    m_pContext = SDL_GL_CreateContext(m_pWindow);
    
    return m_pContext != NULL;
}
