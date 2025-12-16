/**
 * @file Object.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 对象类
 */

#pragma once

#include <SDL_rect.h>
#include <SDL.h>
#include <SDL_stdinc.h>


 struct Player{
    SDL_Texture* m_pTexture = nullptr;
    SDL_FPoint m_fPosition = {0, 0};
    int m_nWidth = 0;
    int m_nHeight = 0;
    float m_fSpeed = 400.0f;
    Uint32 m_nCoolDown = 200;      //SDL指定的时间读取类型为Uint32
    Uint32 m_nLastShootTime = 0;
    int m_nCurrentHealth = 3;
    const static int m_nMaxHealth = 3;
    Uint32 m_nSpeedUpBuffTime = 0;
    Uint32 m_nBulletSpeedUpBuffTime = 0;
    Uint32 m_nShieldHealth = 0;
 };

 struct PlayerBullet{
   SDL_Texture* m_pTexture = nullptr;
   SDL_FPoint m_fPosition = {0, 0};
   int m_nWidth = 0;
   int m_nHeight = 0;
   float m_fSpeed = 600.0f;
   int m_nDamage = 1;
 };

 struct Enemy{
    SDL_Texture* m_pTexture = nullptr;
    SDL_FPoint m_fPosition = {0, 0};
    int m_nWidth = 0;
    int m_nHeight = 0;
    float m_fSpeed = 50.0f;
    Uint32 m_nCoolDown = 4000;      //SDL指定的时间读取类型为Uint32
    Uint32 m_nLastShootTime = 0;
    int m_nCurrentHealth = 2;
 };

 struct EnemyBullet{
    SDL_Texture* m_pTexture = nullptr;
    SDL_FPoint m_fPosition = {0, 0};
    SDL_FPoint m_fDirection = {0, 0};
    int m_nWidth = 0;
    int m_nHeight = 0;
    float m_fSpeed = 400.0f;
    int m_nDamage = 1;
 };

 struct Explosion{
   SDL_Texture* m_pTexture = nullptr;
   SDL_FPoint m_fPosition = {0, 0};
   int m_nWidth = 0;
   int m_nHeight = 0;
   int m_nCurFrame = 0;
   int m_nTotalFrame = 0;
   Uint32 m_nStartTime = 0;
   Uint32 m_nFPS = 10;
 };


 enum ItemType{
   HEALTH,
   SHIELD,
   SPEED_UP,
   BULLET_SPEED_UP,
 };

 struct Item{
   SDL_Texture* m_pTexture = nullptr;
   SDL_FPoint m_fPosition = {0, 0};
   SDL_FPoint m_fDirection = {0, 0};
   int m_nWidth = 0;
   int m_nHeight = 0;
   ItemType m_eType = HEALTH;
   float m_fSpeed = 100.0f;
   int m_nBounceCount = 3;  //反弹次数
 };


struct Background{
  SDL_Texture* m_pTexture = nullptr;
  SDL_FPoint m_fPosition = {0, 0};
  float m_fOffset = 0.0f;
  int m_nWidth = 0;
  int m_nHeight = 0;
  float m_fSpeed = 30.0f;
};
