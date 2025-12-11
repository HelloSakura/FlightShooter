#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif
#include "SceneMain.h"
#include "Game.h"
#include "Object.h"
#include "SDL_image.h"
#include "SDL_rect.h"
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include "SceneTitle.h"

SceneMain::SceneMain() {}
SceneMain::~SceneMain() {}

void SceneMain::init() {
    m_player.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/SpaceShip.png");
    if (m_player.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_player.m_pTexture, nullptr, nullptr, &m_player.m_nWidth, &m_player.m_nHeight);
    m_player.m_nWidth /= 4;
    m_player.m_nHeight /= 4;
    m_player.m_fPosition.y = static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight);
    m_player.m_fPosition.x = static_cast<float>(m_game.getWindowWidth()) / 2.0f - static_cast<float>(m_player.m_nWidth) / 2.0f;

    // 初始化子弹模板
    m_playerBulletTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/laser-3.png");
    if (m_playerBulletTemplate.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_playerBulletTemplate.m_pTexture, nullptr, nullptr, &m_playerBulletTemplate.m_nWidth, &m_playerBulletTemplate.m_nHeight);
    m_playerBulletTemplate.m_nWidth /= 4;
    m_playerBulletTemplate.m_nHeight /= 4;

    // 初始化敌人模板
    m_enemyTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/insect-1.png");
    if (m_enemyTemplate.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_enemyTemplate.m_pTexture, nullptr, nullptr, &m_enemyTemplate.m_nWidth, &m_enemyTemplate.m_nHeight);
    m_enemyTemplate.m_nWidth /= 4;
    m_enemyTemplate.m_nHeight /= 4;

    // 初始化敌人子弹模板
    m_enemyBulletTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/laser-2.png");
    if (m_enemyBulletTemplate.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_enemyBulletTemplate.m_pTexture, nullptr, nullptr, &m_enemyBulletTemplate.m_nWidth, &m_enemyBulletTemplate.m_nHeight);
    m_enemyBulletTemplate.m_nWidth /= 4;
    m_enemyBulletTemplate.m_nHeight /= 4;

    // 初始化爆炸模板
    m_explosionTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/effect/explosion.png");
    if (m_explosionTemplate.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_explosionTemplate.m_pTexture, nullptr, nullptr, &m_explosionTemplate.m_nWidth, &m_explosionTemplate.m_nHeight);
    m_explosionTemplate.m_nTotalFrame = m_explosionTemplate.m_nWidth / m_explosionTemplate.m_nHeight; // 计算总帧数
    m_explosionTemplate.m_nWidth = m_explosionTemplate.m_nHeight;

    // 初始化道具模板
    m_itemTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/bonus_life.png");
    if (m_itemTemplate.m_pTexture == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_itemTemplate.m_pTexture, nullptr, nullptr, &m_itemTemplate.m_nWidth, &m_itemTemplate.m_nHeight);
    m_itemTemplate.m_nWidth /= 4;
    m_itemTemplate.m_nHeight /= 4;
    m_itemTemplate.m_eType = HEALTH;

    // 初始化血量UI
    m_pUIHealth = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/health.png");
    if (m_pUIHealth == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }


    // 初始化随机数
    std::random_device rd;
    m_randomEngine = std::mt19937(rd());
    m_randomDistribution = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // 初始化背景音乐
    m_pBgm = Mix_LoadMUS("../../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (m_pBgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", Mix_GetError());
        m_game.stop();
        return;
    }
    // 播放背景音乐
    Mix_PlayMusic(m_pBgm, -1);

    // 初始化音效
    m_soundEffects["player_shoot"] = Mix_LoadWAV("../../assets/sound/laser_shoot4.wav");
    m_soundEffects["enemy_shoot"] = Mix_LoadWAV("../../assets/sound/xs_laser.wav");
    m_soundEffects["enemy_hit"] = Mix_LoadWAV("../../assets/sound/explosion3.wav");
    m_soundEffects["player_hit"] = Mix_LoadWAV("../../assets/sound/explosion1.wav");
    m_soundEffects["item_pick"] = Mix_LoadWAV("../../assets/sound/eff5.wav");

    //初始化SDL_ttf
    if(TTF_Init() == -1){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL_ttf: %s", TTF_GetError());
        m_game.stop();
        return;
    }

    //加载字体
    m_pScoreFont = TTF_OpenFont("../../assets/font/VonwaonBitmap-12px.ttf", 24);
    if(m_pScoreFont == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", TTF_GetError());
        m_game.stop();
        return;
    }
}

void SceneMain::update(float deltaTime) {
    keyboardControl(deltaTime);
    updatePlayerBullets(deltaTime);
    spawnEnemy(deltaTime);
    updateEnemies(deltaTime);
    updateEnemyBullets(deltaTime);
    updatePlayer(deltaTime);
    updateExplosions(deltaTime);
    updateItems(deltaTime);
}

void SceneMain::updatePlayer(float deltaTime) 
{
    if (m_bIsPlayerDead) {
        return;
    }

    if (m_player.m_nCurrentHealth <= 0) {
        m_bIsPlayerDead = true;
        // 创建爆炸特效
        auto explosion = new Explosion(m_explosionTemplate);
        explosion->m_fPosition.x = m_player.m_fPosition.x + m_player.m_nWidth / 2.0f - explosion->m_nWidth / 2.0f;
        explosion->m_fPosition.y = m_player.m_fPosition.y + m_player.m_nHeight / 2.0f - explosion->m_nHeight / 2.0f;
        explosion->m_nStartTime = SDL_GetTicks();
        m_explosions.push_back(explosion);
        return;
    }

    for (auto enemy : m_enemies) {
        SDL_Rect playerRect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};
        SDL_Rect enemyRect = {static_cast<int>(enemy->m_fPosition.x), static_cast<int>(enemy->m_fPosition.y), enemy->m_nWidth, enemy->m_nHeight};
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            enemy->m_nCurrentHealth = 0;
            if (m_player.m_nShieldHealth > 0) {
                m_player.m_nShieldHealth -= 1;
                SDL_Log("玩家护盾减少1，当前护盾值：%u", m_player.m_nShieldHealth);
            } else {
                m_player.m_nCurrentHealth -= 1;
                SDL_Log("玩家受到伤害，当前生命值：%d", m_player.m_nCurrentHealth);
            }
        }
    }

    Uint32 deltaTimeMs = static_cast<Uint32>(deltaTime * 1000.0f);
    if (m_player.m_nSpeedUpBuffTime > deltaTimeMs) {
        m_player.m_nSpeedUpBuffTime -= deltaTimeMs;
    } else {
        m_player.m_nSpeedUpBuffTime = 0;
    }
    if (m_player.m_nBulletSpeedUpBuffTime > deltaTimeMs) {
        m_player.m_nBulletSpeedUpBuffTime -= deltaTimeMs;
    } else {
        m_player.m_nBulletSpeedUpBuffTime = 0;
    }

    if (m_player.m_nSpeedUpBuffTime > 0) {
        m_player.m_fSpeed = 1000.0f;
    } else {
        m_player.m_fSpeed = 400.0f;
    }

    if (m_player.m_nBulletSpeedUpBuffTime > 0) {
        for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
            PlayerBullet *pBullet = *it;
            pBullet->m_fSpeed = 1500.0f;
        }
    } else {
        for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
            PlayerBullet *pBullet = *it;
            pBullet->m_fSpeed = 600.0f;
        }
    }
}

void SceneMain::updatePlayerBullets(float deltaTime) 
{
    for (auto it = m_playerBullets.begin(); it != m_playerBullets.end();) {
        PlayerBullet *pBullet = *it;
        pBullet->m_fPosition.y -= pBullet->m_fSpeed * deltaTime;
        // 检查子弹是否超出屏幕
        if (pBullet->m_fPosition.y < static_cast<float>(-pBullet->m_nHeight)) {
            delete pBullet;
            it = m_playerBullets.erase(it);
            // SDL_Log("Player Bullet deleted");
        } 
        else {
            bool hitEnemy = false;
            // 检查子弹是否击中敌人
            for (auto enemy : m_enemies) {
                SDL_Rect bulletRect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
                SDL_Rect enemyRect = {static_cast<int>(enemy->m_fPosition.x), static_cast<int>(enemy->m_fPosition.y), enemy->m_nWidth, enemy->m_nHeight};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy->m_nCurrentHealth -= pBullet->m_nDamage;
                    delete pBullet;
                    it = m_playerBullets.erase(it);
                    hitEnemy = true;
                    break;
                }
            }
            if (!hitEnemy) {
                ++it;
            }
        }
    }
}

void SceneMain::updateEnemyBullets(float deltaTime) {
    auto margin = 32.0f;
    for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();) {
        EnemyBullet *pBullet = *it;
        // 检查子弹是否超出屏幕，四个方向都判断
        pBullet->m_fPosition.y += pBullet->m_fSpeed * deltaTime * pBullet->m_fDirection.y;
        pBullet->m_fPosition.x += pBullet->m_fSpeed * deltaTime * pBullet->m_fDirection.x;
        if (pBullet->m_fPosition.y > static_cast<float>(m_game.getWindowHeight()) + margin || pBullet->m_fPosition.y < -margin || pBullet->m_fPosition.x > static_cast<float>(m_game.getWindowWidth()) + margin || pBullet->m_fPosition.x < -margin) {
            delete pBullet;
            it = m_enemyBullets.erase(it);
        } else {
            // 检查子弹是否击中玩家
            SDL_Rect bulletRect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
            SDL_Rect playerRect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};

            if (SDL_HasIntersection(&bulletRect, &playerRect) && !m_bIsPlayerDead) {
                m_player.m_nCurrentHealth -= pBullet->m_nDamage;
                delete pBullet;
                it = m_enemyBullets.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::updateItems(float deltaTime) {
    for (auto it = m_items.begin(); it != m_items.end();) {
        Item *pItem = *it;
        // 更新位置
        pItem->m_fPosition.y += pItem->m_fSpeed * deltaTime * pItem->m_fDirection.y;
        pItem->m_fPosition.x += pItem->m_fSpeed * deltaTime * pItem->m_fDirection.x;

        // 判断是否反弹
        if (pItem->m_nBounceCount > 0) {
            // 碰到左边墙，反弹
            if (pItem->m_fPosition.x < 0) {
                pItem->m_fDirection.x = -pItem->m_fDirection.x;
                pItem->m_nBounceCount--;
            }
            // 碰到右边墙，反弹
            if (pItem->m_fPosition.x > static_cast<float>(m_game.getWindowWidth())) {
                pItem->m_fDirection.x = -pItem->m_fDirection.x;
                pItem->m_nBounceCount--;
            }
            // 碰到上边墙，反弹
            if (pItem->m_fPosition.y < 0) {
                pItem->m_fDirection.y = -pItem->m_fDirection.y;
                pItem->m_nBounceCount--;
            }
            // 碰到下边墙，反弹
            if (pItem->m_fPosition.y > static_cast<float>(m_game.getWindowHeight())) {
                pItem->m_fDirection.y = -pItem->m_fDirection.y;
                pItem->m_nBounceCount--;
            }
        }

        // 判断是否超出屏幕
        if (pItem->m_fPosition.x < -pItem->m_nWidth || pItem->m_fPosition.x > static_cast<float>(m_game.getWindowWidth()) + pItem->m_nWidth || pItem->m_fPosition.y < -pItem->m_nHeight || pItem->m_fPosition.y > static_cast<float>(m_game.getWindowHeight()) + pItem->m_nHeight) {
            delete pItem;
            it = m_items.erase(it);
            continue;
        }

        // 碰到玩家
        SDL_Rect itemRect = {static_cast<int>(pItem->m_fPosition.x), static_cast<int>(pItem->m_fPosition.y), pItem->m_nWidth, pItem->m_nHeight};
        SDL_Rect playerRect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};

        if (SDL_HasIntersection(&itemRect, &playerRect)) {
            playerPickItem(pItem);
            delete pItem;
            it = m_items.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneMain::spawnEnemy(float deltaTime) {
    // 每秒生成一个敌人
    if (m_randomDistribution(m_randomEngine) > 1.0f / 60.0f) {
        return;
    }

    // 使用模板创建敌人
    Enemy *pEnemy = new Enemy(m_enemyTemplate);
    // 创建敌人位置
    pEnemy->m_fPosition.x = static_cast<float>(m_randomDistribution(m_randomEngine) * (static_cast<float>(m_game.getWindowWidth()) - static_cast<float>(pEnemy->m_nWidth)));
    pEnemy->m_fPosition.y = -static_cast<float>(pEnemy->m_nHeight);
    // 添加到敌人列表
    m_enemies.push_back(pEnemy);
}

void SceneMain::updateEnemies(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        Enemy *pEnemy = *it;
        pEnemy->m_fPosition.y += pEnemy->m_fSpeed * deltaTime;
        // 检查敌人是否超出屏幕
        if (pEnemy->m_fPosition.y > static_cast<float>(m_game.getWindowHeight())) {
            delete pEnemy;
            it = m_enemies.erase(it);
        } else {
            // 判断是否发射子弹
            if (pEnemy->m_nLastShootTime + pEnemy->m_nCoolDown < currentTime && !m_bIsPlayerDead) {
                // 发射子弹
                enemyShoot(pEnemy);
                pEnemy->m_nLastShootTime = currentTime;
            }

            // 判断敌人是否死亡
            if (pEnemy->m_nCurrentHealth <= 0) {
                explodeEnemy(pEnemy);
                it = m_enemies.erase(it);
                // 播放音效
                Mix_PlayChannel(3, m_soundEffects["enemy_hit"], 0);
            } else {
                ++it;
            }
        }
    }
}

void SceneMain::updateExplosions(float deltaTime) {
    for (auto it = m_explosions.begin(); it != m_explosions.end();) {
        Explosion *pExplosion = *it;
        auto currentTime = SDL_GetTicks();
        auto elapsedTime = currentTime - pExplosion->m_nStartTime;
        auto frame = elapsedTime / 1000.0f * pExplosion->m_nFPS;
        pExplosion->m_nCurFrame = static_cast<int>(frame);
        if (pExplosion->m_nCurFrame >= pExplosion->m_nTotalFrame) {
            delete pExplosion;
            it = m_explosions.erase(it);
        } else {
            ++it;
        }
    }
}

void SceneMain::explodeEnemy(Enemy *pEnemy) {
    // 使用模板创建爆炸
    Explosion *pExplosion = new Explosion(m_explosionTemplate);
    // 创建爆炸位置
    pExplosion->m_fPosition.x = pEnemy->m_fPosition.x + pEnemy->m_nWidth / 2.0f - pExplosion->m_nWidth / 2.0f;
    pExplosion->m_fPosition.y = pEnemy->m_fPosition.y + pEnemy->m_nHeight / 2.0f - pExplosion->m_nHeight / 2.0f;
    pExplosion->m_nStartTime = SDL_GetTicks();
    // 添加到爆炸列表
    m_explosions.push_back(pExplosion);
    // 掉落物品 50%概率掉落
    if (m_randomDistribution(m_randomEngine) < 0.5f) {
        dropItem(pEnemy);
    }
    // 删除敌人
    delete pEnemy;
    pEnemy = nullptr;
    // 加分
    m_nScore += 10;
}

void SceneMain::enemyShoot(Enemy *pEnemy) {
    // 使用模板创建子弹
    EnemyBullet *pBullet = new EnemyBullet(m_enemyBulletTemplate);
    // 创建子弹位置
    pBullet->m_fPosition.x = pEnemy->m_fPosition.x + pEnemy->m_nWidth / 2.0f - pBullet->m_nWidth / 2.0f;
    pBullet->m_fPosition.y = pEnemy->m_fPosition.y + pEnemy->m_nHeight / 2.0f - pBullet->m_nHeight / 2.0f;
    // 设置子弹方向
    pBullet->m_fDirection = getDirection(pEnemy);
    // 添加到子弹列表
    m_enemyBullets.push_back(pBullet);
    // 播放音效
    Mix_PlayChannel(1, m_soundEffects["enemy_shoot"], 0);
}

void SceneMain::dropItem(Enemy *pEnemy) {
    // 使用模板创建道具
    ItemType itemType = static_cast<ItemType>(m_randomDistribution(m_randomEngine) * 4);
    Item *pItem = new Item(m_itemTemplate);
    pItem->m_eType = itemType;
    // 创建道具位置
    pItem->m_fPosition.x = pEnemy->m_fPosition.x + pEnemy->m_nWidth / 2.0f - pItem->m_nWidth / 2.0f;
    pItem->m_fPosition.y = pEnemy->m_fPosition.y + pEnemy->m_nHeight / 2.0f - pItem->m_nHeight / 2.0f;
    // 设置道具方向
    float angle = m_randomDistribution(m_randomEngine) * 2 * M_PI;
    pItem->m_fDirection = {cosf(angle), -sinf(angle)};
    // 添加到道具列表
    m_items.push_back(pItem);
}

SDL_FPoint SceneMain::getDirection(Enemy *pEnemy) {
    auto x = (m_player.m_fPosition.x + m_player.m_nWidth / 2.0f) - (pEnemy->m_fPosition.x + pEnemy->m_nWidth / 2.0f);
    auto y = (m_player.m_fPosition.y + m_player.m_nHeight / 2.0f) - (pEnemy->m_fPosition.y + pEnemy->m_nHeight / 2.0f);
    auto length = sqrtf(x * x + y * y);
    return {x / length, y / length};
}

void SceneMain::render() {
    // 渲染子弹
    renderPlayerProjectiles();
    // 渲染玩家
    if (!m_bIsPlayerDead) {
        SDL_Rect rect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), m_player.m_pTexture, nullptr, &rect);
    }
    // 渲染敌人
    renderEnemies();
    // 渲染敌人子弹
    renderEnemyBullets();
    // 渲染爆炸
    renderExplosions();
    // 渲染道具
    renderItems();
    // 渲染血量UI
    renderUI();
}

void SceneMain::renderPlayerProjectiles() {
    for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
        PlayerBullet *pBullet = *it;
        SDL_Rect rect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pBullet->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::renderEnemies() {
    for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it) {
        Enemy *pEnemy = *it;
        SDL_Rect rect = {static_cast<int>(pEnemy->m_fPosition.x), static_cast<int>(pEnemy->m_fPosition.y), pEnemy->m_nWidth, pEnemy->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pEnemy->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::renderEnemyBullets() {
    for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end(); ++it) {
        EnemyBullet *pBullet = *it;
        SDL_Rect rect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
        // 旋转子弹
        float angle = atan2f(pBullet->m_fDirection.y, pBullet->m_fDirection.x) * 180.0f / M_PI - 90.0f;
        SDL_RenderCopyEx(m_game.getRenderer(), pBullet->m_pTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::renderExplosions() {
    for (auto it = m_explosions.begin(); it != m_explosions.end(); ++it) {
        Explosion *pExplosion = *it;
        // 决定渲染区域
        // 图片区
        SDL_Rect srcRect = {static_cast<int>(pExplosion->m_nCurFrame * pExplosion->m_nWidth), 0, pExplosion->m_nWidth, pExplosion->m_nHeight};
        // 渲染区域
        SDL_Rect dstRect = {static_cast<int>(pExplosion->m_fPosition.x), static_cast<int>(pExplosion->m_fPosition.y), pExplosion->m_nWidth, pExplosion->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pExplosion->m_pTexture, &srcRect, &dstRect);
    }
}

void SceneMain::renderItems() {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        Item *pItem = *it;
        SDL_Rect rect = {static_cast<int>(pItem->m_fPosition.x), static_cast<int>(pItem->m_fPosition.y), pItem->m_nWidth, pItem->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pItem->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::renderUI() 
{
  //渲染血条
  int x = 10, y = 10;
  int size = 32;
  int offset = 40;

  SDL_SetTextureColorMod(m_pUIHealth, 100, 100, 100);
  for(int i = 0; i < m_player.m_nMaxHealth; i++) {
    SDL_Rect rect = {x + i * offset, y, size, size};
    SDL_RenderCopy(m_game.getRenderer(), m_pUIHealth, nullptr, &rect);
  }

  SDL_SetTextureColorMod(m_pUIHealth, 255, 255, 255);
  for(int i = 0; i < m_player.m_nCurrentHealth; i++) {
    SDL_Rect rect = {x + i * offset, y, size, size};
    SDL_RenderCopy(m_game.getRenderer(), m_pUIHealth, nullptr, &rect);
  }

  //渲染得分
  std::stringstream ss;
  ss << "SCORE:" << m_nScore;
  std::string text = ss.str();
  SDL_Color color = {255, 255, 255, 255};
  SDL_Surface* pTextSurface = TTF_RenderUTF8_Solid(m_pScoreFont, text.c_str(), color);
  SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(m_game.getRenderer(), pTextSurface);
  SDL_Rect rect = {m_game.getWindowWidth() - 160, 10, pTextSurface->w, pTextSurface->h};
  SDL_RenderCopy(m_game.getRenderer(), pTextTexture, nullptr, &rect);
  SDL_FreeSurface(pTextSurface);
  SDL_DestroyTexture(pTextTexture);
}

void SceneMain::clean() {
    // 清理容器
    // 清理子弹
    for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    m_playerBullets.clear();

    // 清理敌人
    for (auto it = m_enemies.begin(); it != m_enemies.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    m_enemies.clear();

    // 清理敌人子弹
    for (auto it = m_enemyBullets.begin(); it != m_enemyBullets.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    m_enemyBullets.clear();

    // 清理爆炸
    for (auto it = m_explosions.begin(); it != m_explosions.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    m_explosions.clear();

    // 清理道具
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if (*it != nullptr) {
            delete *it;
            *it = nullptr;
        }
    }
    m_items.clear();

    // 清理模板
    if (m_explosionTemplate.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_explosionTemplate.m_pTexture);
        m_explosionTemplate.m_pTexture = nullptr;
    }

    if (m_enemyTemplate.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_enemyTemplate.m_pTexture);
        m_enemyTemplate.m_pTexture = nullptr;
    }

    if (m_enemyBulletTemplate.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_enemyBulletTemplate.m_pTexture);
        m_enemyBulletTemplate.m_pTexture = nullptr;
    }

    if (m_playerBulletTemplate.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_playerBulletTemplate.m_pTexture);
        m_playerBulletTemplate.m_pTexture = nullptr;
    }
    if (m_itemTemplate.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_itemTemplate.m_pTexture);
        m_itemTemplate.m_pTexture = nullptr;
    }

    // 清理玩家
    if (m_player.m_pTexture != nullptr) {
        SDL_DestroyTexture(m_player.m_pTexture);
        m_player.m_pTexture = nullptr;
    }

    // 清理血量UI
    if (m_pUIHealth != nullptr) {
        SDL_DestroyTexture(m_pUIHealth);
        m_pUIHealth = nullptr;
    }

    // 清理音乐
    if (m_pBgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(m_pBgm);
        m_pBgm = nullptr;
    }

    // 清理音效
    for (auto it = m_soundEffects.begin(); it != m_soundEffects.end(); ++it) {
        if (it->second != nullptr) {
            Mix_FreeChunk(it->second);
            it->second = nullptr;
        }
    }
    m_soundEffects.clear();

    // 清理SDL_ttf
    TTF_Quit();
    if (m_pScoreFont != nullptr) {
        TTF_CloseFont(m_pScoreFont);
        m_pScoreFont = nullptr;
    }
}

void SceneMain::handleEvents(SDL_Event *pEvent) 
{
    if(pEvent->type == SDL_KEYDOWN){
        if(pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
            m_game.changeScene(new SceneTitle());
        }
    }
}

void SceneMain::keyboardControl(float deltaTime) {
    if (m_bIsPlayerDead) {
        return;
    }

    auto keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
        m_player.m_fPosition.y -= m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]) {
        m_player.m_fPosition.y += m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
        m_player.m_fPosition.x -= m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
        m_player.m_fPosition.x += m_player.m_fSpeed * deltaTime;
    }

    // 控制移动范围
    if (m_player.m_fPosition.x < 0) {
        m_player.m_fPosition.x = 0;
    }

    if (m_player.m_fPosition.x > static_cast<float>(m_game.getWindowWidth() - m_player.m_nWidth)) {
        m_player.m_fPosition.x = static_cast<float>(m_game.getWindowWidth() - m_player.m_nWidth);
    }

    if (m_player.m_fPosition.y < 0) {
        m_player.m_fPosition.y = 0;
    }

    if (m_player.m_fPosition.y > static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight)) {
        m_player.m_fPosition.y = static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight);
    }

    // 控制子弹发射
    if (keyboardState[SDL_SCANCODE_SPACE]) {
        auto currentTime = SDL_GetTicks();
        if (currentTime - m_player.m_nLastShootTime > m_player.m_nCoolDown) {
            shootPlayerBullet();
            m_player.m_nLastShootTime = currentTime;
        }
    }
}

void SceneMain::shootPlayerBullet() {
    // 使用模板创建子弹
    PlayerBullet *pBullet = new PlayerBullet(m_playerBulletTemplate);
    // 创建子弹位置
    pBullet->m_fPosition.x = m_player.m_fPosition.x + m_player.m_nWidth / 2.0f - pBullet->m_nWidth / 2.0f;
    pBullet->m_fPosition.y = m_player.m_fPosition.y;
    // 添加到子弹列表
    m_playerBullets.push_back(pBullet);
    // 播放音效
    Mix_PlayChannel(0, m_soundEffects["player_shoot"], 0);
}

void SceneMain::playerPickItem(Item *pItem) {
    switch (pItem->m_eType) {
    case HEALTH:
        m_player.m_nCurrentHealth += 1;
        m_player.m_nCurrentHealth = std::min(m_player.m_nCurrentHealth, m_player.m_nMaxHealth);
        SDL_Log("玩家拾取了生命值道具，当前生命值：%d", m_player.m_nCurrentHealth);
        break;
    case SHIELD:
        m_player.m_nShieldHealth += 5;
        SDL_Log("玩家拾取了护盾道具，当前护盾值：%u", m_player.m_nShieldHealth);
        break;
    case SPEED_UP:
        m_player.m_nSpeedUpBuffTime = 1000;
        SDL_Log("玩家拾取了速度道具，当前速度：%.2f", m_player.m_fSpeed);
        break;
    case BULLET_SPEED_UP:
        m_player.m_nBulletSpeedUpBuffTime = 1000;
        SDL_Log("玩家拾取了子弹速度道具");
        break;
    default:
        SDL_Log("玩家拾取了未知道具，类型：%d", static_cast<int>(pItem->m_eType));
        break;
    }

    // 播放音效
    Mix_PlayChannel(4, m_soundEffects["item_pick"], 0);
    // 加分
    m_nScore += 5;
}