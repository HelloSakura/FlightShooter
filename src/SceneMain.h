/**
 * @file SceneMain.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 主场景类
 */

#pragma once

#include "Scene.h"
#include "Object.h"
#include <list>

class Game;

class SceneMain : public Scene{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* pEvent) override;
    void keyboardControl(float deltaTime);
    void shootPlayerBullet();
    void updatePlayerBullets(float deltaTime);

    void renderPlayerProjectiles();

private:
    Game& m_game;
    Player m_player;
    //创建每个物体的模板，避免频繁读取文件
    PlayerBullet m_playerBulletTemplate;
    
    std::list<PlayerBullet*> m_pPlayerBullets;   //子弹列表，使用指针避免栈上内存过多


};
