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
#include <random>

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
    void spawnEnemy(float deltaTime);
    void updateEnemies(float deltaTime);

    void renderPlayerProjectiles();
    void renderEnemies();
private:
    Game& m_game;
    Player m_player;
    //创建每个物体的模板，避免频繁读取文件
    PlayerBullet m_playerBulletTemplate;
    Enemy m_enemyTemplate;
    
    std::list<PlayerBullet*> m_pPlayerBullets;   //子弹列表，使用指针避免栈上内存过多
    std::list<Enemy*> m_pEnemies;   //敌人列表，使用指针避免栈上内存过多

    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<float> m_randomDistribution;
};
