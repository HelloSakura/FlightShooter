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

    //初始化子弹模板
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

    //初始化敌人模板
    m_enemyTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/insect-1.png");
    if(m_enemyTemplate.m_pTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_enemyTemplate.m_pTexture, nullptr, nullptr, &m_enemyTemplate.m_nWidth, &m_enemyTemplate.m_nHeight);
    m_enemyTemplate.m_nWidth /= 4;
    m_enemyTemplate.m_nHeight /= 4;

    //初始化敌人子弹模板
    m_enemyBulletTemplate.m_pTexture = IMG_LoadTexture(m_game.getRenderer(), "../../assets/image/laser-2.png");
    if(m_enemyBulletTemplate.m_pTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", IMG_GetError());
        m_game.stop();
        return;
    }
    SDL_QueryTexture(m_enemyBulletTemplate.m_pTexture, nullptr, nullptr, &m_enemyBulletTemplate.m_nWidth, &m_enemyBulletTemplate.m_nHeight);
    m_enemyBulletTemplate.m_nWidth /= 4;
    m_enemyBulletTemplate.m_nHeight /= 4;

    //初始化随机数
    std::random_device rd;
    m_randomEngine = std::mt19937(rd());
    m_randomDistribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerBullets(deltaTime);
    spawnEnemy(deltaTime);
    updateEnemies(deltaTime);
    updateEnemyBullets(deltaTime);
}

void SceneMain::updatePlayerBullets(float deltaTime)
{
    for(auto it = m_playerBullets.begin(); it != m_playerBullets.end();){
        PlayerBullet* pBullet = *it;
        pBullet->m_fPosition.y -= pBullet->m_fSpeed * deltaTime;
        //检查子弹是否超出屏幕
        if(pBullet->m_fPosition.y < static_cast<float>(-pBullet->m_nHeight)){
            delete pBullet;
            it = m_playerBullets.erase(it);
            //SDL_Log("Player Bullet deleted");
        }
        else{
            ++it;
        }

    }
}

void SceneMain::updateEnemyBullets(float deltaTime)
{
    auto margin = 32.0f;
    for(auto it = m_enemyBullets.begin(); it != m_enemyBullets.end();){
        EnemyBullet* pBullet = *it;
        //检查子弹是否超出屏幕，四个方向都判断
        pBullet->m_fPosition.y += pBullet->m_fSpeed * deltaTime * pBullet->m_fDirection.y;
        pBullet->m_fPosition.x += pBullet->m_fSpeed * deltaTime * pBullet->m_fDirection.x;
        if(pBullet->m_fPosition.y > static_cast<float>(m_game.getWindowHeight()) + margin
        || pBullet->m_fPosition.y < -margin
        || pBullet->m_fPosition.x > static_cast<float>(m_game.getWindowWidth()) + margin
        || pBullet->m_fPosition.x < -margin){
            delete pBullet;
            it = m_enemyBullets.erase(it);
        }
        else{
            ++it;
        }
    }
}

void SceneMain::spawnEnemy(float deltaTime)
{
    //每秒生成一个敌人
    if(m_randomDistribution(m_randomEngine) > 1.0f / 60.0f){
        return;
    }

    //使用模板创建敌人
    Enemy* pEnemy = new Enemy(m_enemyTemplate);
    //创建敌人位置
    pEnemy->m_fPosition.x = static_cast<float>(m_randomDistribution(m_randomEngine) * (static_cast<float>(m_game.getWindowWidth()) - static_cast<float>(pEnemy->m_nWidth)));
    pEnemy->m_fPosition.y = -static_cast<float>(pEnemy->m_nHeight);
    //添加到敌人列表
    m_enemies.push_back(pEnemy); 
}

void SceneMain::updateEnemies(float deltaTime)
{
    auto currentTime = SDL_GetTicks();
    for(auto it = m_enemies.begin(); it != m_enemies.end();){
        Enemy* pEnemy = *it;
        pEnemy->m_fPosition.y += pEnemy->m_fSpeed * deltaTime;
        //检查敌人是否超出屏幕
        if(pEnemy->m_fPosition.y > static_cast<float>(m_game.getWindowHeight())){
            delete pEnemy;
            it = m_enemies.erase(it);
        }
        else{
            //判断是否发射子弹
            if(pEnemy->m_nLastShootTime + pEnemy->m_nCoolDown < currentTime){
                //发射子弹
                enemyShoot(pEnemy);
                pEnemy->m_nLastShootTime = currentTime;
            }
            ++it;
        }
    }
}

void SceneMain::enemyShoot(Enemy* pEnemy)
{
    //使用模板创建子弹
    EnemyBullet* pBullet = new EnemyBullet(m_enemyBulletTemplate);
    //创建子弹位置
    pBullet->m_fPosition.x = pEnemy->m_fPosition.x + pEnemy->m_nWidth / 2.0f - pBullet->m_nWidth / 2.0f;
    pBullet->m_fPosition.y = pEnemy->m_fPosition.y + pEnemy->m_nHeight/2.0f - pBullet->m_nHeight/2.0f;
    //设置子弹方向
    pBullet->m_fDirection = getDirection(pEnemy);
    //添加到子弹列表
    m_enemyBullets.push_back(pBullet);
}

SDL_FPoint SceneMain::getDirection(Enemy *pEnemy)
{
    auto x = (m_player.m_fPosition.x + m_player.m_nWidth/2.0f) - (pEnemy->m_fPosition.x + pEnemy->m_nWidth/2.0f);
    auto y = (m_player.m_fPosition.y + m_player.m_nHeight/2.0f) - (pEnemy->m_fPosition.y + pEnemy->m_nHeight/2.0f);
    auto length = sqrtf(x * x + y * y); 
    return {x / length, y / length};
}

void SceneMain::render()
{
    //渲染子弹
    renderPlayerProjectiles();
    //渲染玩家
    SDL_Rect rect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};
    SDL_RenderCopy(m_game.getRenderer(), m_player.m_pTexture, nullptr, &rect);
    //渲染敌人
    renderEnemies();
    //渲染敌人子弹
    renderEnemyBullets();
}

void SceneMain::renderPlayerProjectiles()
{
    for(auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it){
        PlayerBullet* pBullet = *it;
        SDL_Rect rect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pBullet->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::renderEnemies()
{
    for(auto it = m_enemies.begin(); it != m_enemies.end(); ++it){
        Enemy* pEnemy = *it;
        SDL_Rect rect = {static_cast<int>(pEnemy->m_fPosition.x), static_cast<int>(pEnemy->m_fPosition.y), pEnemy->m_nWidth, pEnemy->m_nHeight};
        SDL_RenderCopy(m_game.getRenderer(), pEnemy->m_pTexture, nullptr, &rect);
    }
}

void SceneMain::renderEnemyBullets()
{
    for(auto it = m_enemyBullets.begin(); it != m_enemyBullets.end(); ++it){
        EnemyBullet* pBullet = *it;
        SDL_Rect rect = {static_cast<int>(pBullet->m_fPosition.x), static_cast<int>(pBullet->m_fPosition.y), pBullet->m_nWidth, pBullet->m_nHeight};
        //旋转子弹
        float angle = atan2f(pBullet->m_fDirection.y, pBullet->m_fDirection.x) * 180.0f / M_PI - 90.0f;
        SDL_RenderCopyEx(m_game.getRenderer(), pBullet->m_pTexture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE); 
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
    for(auto it = m_playerBullets.begin(); it != m_playerBullets.end(); ++it){
        if(*it != nullptr)
        {
            delete *it;
            *it = nullptr;
        }
    }
    m_playerBullets.clear();
    
    //清理敌人
    for(auto it = m_enemies.begin(); it != m_enemies.end(); ++it){
        if(*it != nullptr)
        {
            delete *it;
            *it = nullptr;
        }
    }
    m_enemies.clear();

    //清理敌人子弹
    for(auto it = m_enemyBullets.begin(); it != m_enemyBullets.end(); ++it){
        if(*it != nullptr)
        {
            delete *it;
            *it = nullptr;
        }
    }
    m_enemyBullets.clear();
    
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
    m_playerBullets.push_back(pBullet);
}