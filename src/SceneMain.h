/**
 * @file SceneMain.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 主场景类
 */

#pragma once

#include "Scene.h"
#include "Object.h"
#include <SDL_render.h>
#include <list>
#include <random>
#include <SDL_mixer.h>
#include <map>
#include <SDL_ttf.h>

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

    
private://更新逻辑
    void updatePlayer(float deltaTime);
    void updatePlayerBullets(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateEnemyBullets(float deltaTime);
    void updateExplosions(float deltaTime);
    void updateItems(float deltaTime);
    void spawnEnemy(float deltaTime);   //生成敌人

private://渲染逻辑
    
    void renderPlayerProjectiles();
    void renderEnemies();
    void renderEnemyBullets();
    void renderExplosions();
    void renderItems();
    void renderUI();
    
private://辅助函数
    
    void keyboardControl(float deltaTime);
    void shootPlayerBullet();
    void enemyShoot(Enemy* pEnemy);
    void explodeEnemy(Enemy* pEnemy);
    void dropItem(Enemy* pEnemy);
    void playerPickItem(Item* pItem);
    SDL_FPoint getDirection(Enemy *pEnemy);


private:
    Player m_player;
    bool m_bIsPlayerDead = false;
    //创建每个物体的模板，避免频繁读取文件
    PlayerBullet m_playerBulletTemplate;
    Enemy m_enemyTemplate;
    EnemyBullet m_enemyBulletTemplate;
    Explosion m_explosionTemplate;
    Item m_itemTemplate;
    Mix_Music* m_pBgm = nullptr;   //背景音乐
    std::map<std::string, Mix_Chunk*> m_soundEffects;   //音效列表
    SDL_Texture* m_pUIHealth = nullptr;  //血量UI
    TTF_Font* m_pScoreFont = nullptr;  //字体
    int m_nScore = 0;  //分数

    std::list<PlayerBullet*> m_playerBullets;   //子弹列表，使用指针避免栈上内存过多
    std::list<Enemy*> m_enemies;   //敌人列表，使用指针避免栈上内存过多
    std::list<EnemyBullet*> m_enemyBullets;   //敌人子弹列表，使用指针避免栈上内存过多s
    std::list<Explosion*> m_explosions;   //爆炸列表，使用指针避免栈上内存过多
    std::list<Item*> m_items;   //道具列表，使用指针避免栈上内存过多

    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<float> m_randomDistribution;
};
