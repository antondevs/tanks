//
//  main.cpp
//  Tanks
//
//  Created by antondev on 06/12/16.
//  Copyright © 2016 games. All rights reserved.
//

#include "GameCore.h"

int main(int argc, char *argv[])
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL init error: %s\n", SDL_GetError());
        return 0;
    }
    
    GameCore *pGameCore = new GameCore();
    
    if (pGameCore->Init())
    {
        pGameCore->Start();
    }
        else
    {
        printf("Engine start error");
    }
    
    delete pGameCore;
    
    SDL_Quit();
    
    return 0;
}
