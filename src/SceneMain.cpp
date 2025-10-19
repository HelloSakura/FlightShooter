#include "SceneMain.h"
#include "Game.h"
#include "Object.h"
#include "SDL_image.h"


SceneMain::SceneMain() : m_game(Game::getInstance())
{

}

SceneMain::~SceneMain()
{

}

void SceneMain::init()
{
    m_player.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/SpaceShip.png");
    if(m_player.m_pTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_player.m_pTexture, nullptr, nullptr, &m_player.m_nWidth, &m_player.m_nHeight);
    m_player.m_nWidth /= 4;
    m_player.m_nHeight /= 4;
    m_player.m_fPosition.y = static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight);
    m_player.m_fPosition.x = static_cast<float>(m_game.getWindowWidth()) / 2.0f - static_cast<float>(m_player.m_nWidth) / 2.0f;

    //初始化模板
    m_playerBulletTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/laser-3.png");
    if(m_playerBulletTemplate.m_pTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_playerBulletTemplate.m_pTexture, nullptr, nullptr, &m_playerBulletTemplate.m_nWidth, &m_playerBulletTemplate.m_nHeight);
    m_playerBulletTemplate.m_nWidth /= 4;
    m_playerBulletTemplate.m_nHeight /= 4;
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerBullets(deltaTime);
}

void SceneMain::updatePlayerBullets(float deltaTime)
{
    for(auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end();){
        PlayerBullet* pBullet = *it;
        pBullet->m_fPosition.y -= pBullet->m_fSpeed * deltaTime;
        //检查子弹是否超出屏幕
        if(pBullet->m_fPosition.y < static_cast<float>(-pBullet->m_nHeight)){
            delete pBullet;
            it = m_pPlayerBullets.erase(it);
            SDL_Log("Player Bullet deleted");
        }
        else{
            ++it;
        }

    }
}

void SceneMain::render()
{
    //渲染子弹
    renderPlayerProjectiles();
    //渲染玩家
    SDL_Rect rect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};
    SDL_RenderCopy(m_game.getRenderer(), m_player.m_pTexture, nullptr, &rect);
    
}

void SceneMain::renderPlayerProjectiles()
{
    for(auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end(); ++it){
        PlayerBullet* pBullet = *it;
        SDL_Rect rect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pBullet->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::clean()
{
    //清理模板
    if(m_playerBulletTemplate.m_pTexture != nullptr)
    {
        SDL_DestroyTexture(m_playerBulletTemplate.m_pTexture);
        m_playerBulletTemplate.m_pTexture = nullptr;
    }


    //清理子弹
    for(auto it = m_pPlayerBullets.begin(); it != m_pPlayerBullets.end(); ++it){
        if(*it != nullptr)
        {
            delete *it;
            *it = nullptr;
        }
    }
    m_pPlayerBullets.clear();
    //清理玩家
    if(m_player.m_pTexture != nullptr)
    {
        SDL_DestroyTexture(m_player.m_pTexture);
        m_player.m_pTexture = nullptr;
    }

   
}

void SceneMain::handleEvents(SDL_Event* pEvent)
{

}

void SceneMain::keyboardControl(float deltaTime)
{
    auto keyboardState = SDL_GetKeyboardState(nullptr);
    
    if(keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
    {
        m_player.m_fPosition.y -= m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
    {
        m_player.m_fPosition.y += m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
    {
        m_player.m_fPosition.x -= m_player.m_fSpeed * deltaTime;
    }

    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
    {
        m_player.m_fPosition.x += m_player.m_fSpeed * deltaTime;
    }

    //控制移动范围
    if(m_player.m_fPosition.x < 0)
    {
        m_player.m_fPosition.x = 0;
    }

    if(m_player.m_fPosition.x > static_cast<float>(m_game.getWindowWidth() - m_player.m_nWidth))
    {
        m_player.m_fPosition.x = static_cast<float>(m_game.getWindowWidth() - m_player.m_nWidth);
    }

    if(m_player.m_fPosition.y < 0)
    {
        m_player.m_fPosition.y = 0;
    }
    
    if(m_player.m_fPosition.y > static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight))
    {
        m_player.m_fPosition.y = static_cast<float>(m_game.getWindowHeight() - m_player.m_nHeight);
    }
    
    //控制子弹发射
    if(keyboardState[SDL_SCANCODE_SPACE]){
        auto currentTime = SDL_GetTicks();
        if(currentTime - m_player.m_nLastShootTime > m_player.m_nCoolDown){
            shootPlayerBullet();
            m_player.m_nLastShootTime = currentTime;
        }
    }
}

void SceneMain::shootPlayerBullet()
{
    //使用模板创建子弹
    PlayerBullet* pBullet = new PlayerBullet(m_playerBulletTemplate);
    //创建子弹位置
    pBullet->m_fPosition.x = m_player.m_fPosition.x + m_player.m_nWidth / 2.0f - pBullet->m_nWidth / 2.0f;
    pBullet->m_fPosition.y = m_player.m_fPosition.y;
    //添加到子弹列表
    m_pPlayerBullets.push_back(pBullet);
}