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
    float m_fSpeed = 200.0f;
    Uint32 m_nCoolDown = 500;      //SDL指定的时间读取类型为Uint32
    Uint32 m_nLastShootTime = 0;
 };

 struct PlayerBullet{
   SDL_Texture* m_pTexture = nullptr;
   SDL_FPoint m_fPosition = {0, 0};
   int m_nWidth = 0;
   int m_nHeight = 0;
   float m_fSpeed = 400.0f;
 };
