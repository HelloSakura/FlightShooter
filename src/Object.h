/**
 * @file Object.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 对象类
 */

#pragma once

#include <SDL_rect.h>
#include <SDL.h>


 struct Player{
    SDL_Texture* m_pTexture = nullptr;
    SDL_FPoint m_fPosition = {0, 0};
    int m_nWidth = 0;
    int m_nHeight = 0;
 };

 
