#include "Game.h"
#include "SDL_log.h" 
#include <SDL.h>
#include <SDL_stdinc.h>
#include "SDL_image.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "SceneMain.h"
#include "SceneTitle.h"

int Game::sm_nWindowWidth = 600;
int Game::sm_nWindowHeight = 800;

Game::Game()
{

}

Game::~Game()
{
    clean();
}

void Game::run()
{
    //主循环
    while(m_isRunning)
    {
        Uint32 frameStart = SDL_GetTicks();
        
        // 处理所有事件
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            // 先处理全局事件（如退出）
            handleEvents(&event);
            // 然后处理场景特定事件
            m_pCurScene->handleEvents(&event);
        }
        
        update(m_fDeltaTime);
        render();
        
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 diff = frameEnd - frameStart;
        //等待差值时间，保证帧率
        if(diff < m_nFrameTime){
            SDL_Delay(m_nFrameTime - diff);
            m_fDeltaTime = static_cast<float>(m_nFrameTime - diff) / 1000.0f;
        }
        else{
            //没有进行等待，低于设定帧率
            m_fDeltaTime = static_cast<float>(diff) / 1000.0f;
        }
    }
}

void Game::handleEvents(SDL_Event* pEvent)
{
    //处理退出事件
    if(pEvent->type == SDL_QUIT)
    {
        m_isRunning = false;
    }
}


void Game::update(float deltaTime)
{
    updateBackground(deltaTime);
    m_pCurScene->update(deltaTime);
}

void Game::updateBackground(float deltaTime)
{
    m_nearStars.m_fOffset += m_nearStars.m_fSpeed * deltaTime;
    if(m_nearStars.m_fOffset >= 0){
        m_nearStars.m_fOffset -= m_nearStars.m_nWidth;
    }

    m_farStars.m_fOffset += m_farStars.m_fSpeed * deltaTime;
    if(m_farStars.m_fOffset >= 0){
        m_farStars.m_fOffset -= m_farStars.m_nWidth;
    }
   
}

void Game::render()
{
    //清空渲染场景
    SDL_RenderClear(m_pRenderer);
    //渲染星空背景
    renderBackground();
    //绘制场景内容
    m_pCurScene->render();
    //更新渲染场景
    SDL_RenderPresent(m_pRenderer);
}


void Game::renderBackground()
{
    //根据offset获取Y坐标
    int posY = static_cast<int>(m_nearStars.m_fOffset);
    for(; posY < sm_nWindowHeight; posY += m_nearStars.m_nHeight){
        for(int posX = 0; posX < sm_nWindowWidth; posX += m_nearStars.m_nWidth){
            SDL_Rect dstRect = {posX, posY, m_nearStars.m_nWidth, m_nearStars.m_nHeight};
            SDL_RenderCopy(m_pRenderer, m_nearStars.m_pTexture, nullptr, &dstRect);
        }
    }

}

void Game::changeScene(Scene* pScene)
{
    if(m_pCurScene != nullptr)
    {
        m_pCurScene->clean();
        delete m_pCurScene;
    }
    m_pCurScene = pScene;
    m_pCurScene->init();
}

void Game::init()
{
    m_nFrameTime  = 1000 / m_nFPS;
    //SDL初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_Init Error: %s", SDL_GetError());
        m_isRunning = false;
        return;
    }

    //创建窗口
    m_pWindow = SDL_CreateWindow("FlightShooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sm_nWindowWidth, sm_nWindowHeight, SDL_WINDOW_SHOWN);
    if(m_pWindow == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateWindow Error: %s", SDL_GetError());
        m_isRunning = false;
        return;
    }

    //创建渲染器
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(m_pRenderer == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateRenderer Error: %s", SDL_GetError());
        m_isRunning = false;
        return;
    }

    //初始化图片
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG)){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_Init Error: %s", IMG_GetError());
        m_isRunning = false;
        return;
    }

    //初始化音乐
    if(Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG) != (MIX_INIT_MP3|MIX_INIT_OGG)){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Mix_Init Error: %s", Mix_GetError());
        m_isRunning = false;
        return;
    }   

    //打开音频设备
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Mix_OpenAudio Error: %s", Mix_GetError());
        m_isRunning = false;
        return;
    }

    //设置通道
    Mix_AllocateChannels(32);
    //设置音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    //初始化背景卷轴
    m_nearStars.m_pTexture = IMG_LoadTexture(m_pRenderer, "../../assets/image/StarsA.png");
    if(m_nearStars.m_pTexture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_LoadTexture Error: %s", IMG_GetError());
        m_isRunning = false;
        return;
    }
    SDL_QueryTexture(m_nearStars.m_pTexture, nullptr, nullptr, &m_nearStars.m_nWidth, &m_nearStars.m_nHeight);
    SDL_Log("m_nearStars.m_nWidth: %d, m_nearStars.m_nHeight: %d", m_nearStars.m_nWidth, m_nearStars.m_nHeight);

    m_farStars.m_pTexture = IMG_LoadTexture(m_pRenderer, "../../assets/image/StarsB.png");
    if(m_farStars.m_pTexture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_LoadTexture Error: %s", IMG_GetError());
        m_isRunning = false;
        return;
    }
    SDL_QueryTexture(m_farStars.m_pTexture, nullptr, nullptr, &m_farStars.m_nWidth, &m_farStars.m_nHeight);
    SDL_Log("m_farStars.m_nWidth: %d, m_farStars.m_nHeight: %d", m_farStars.m_nWidth, m_farStars.m_nHeight);
    m_farStars.m_fSpeed = 20.0f;

    //初始化字体
    if(TTF_Init() == -1){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"TTF_Init Error: %s", TTF_GetError());
        m_isRunning = false;
        return;
    }

    m_pTitleFont = TTF_OpenFont("../../assets/font/VonwaonBitmap-16px.ttf", 64);
    if(m_pTitleFont == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"TTF_OpenFont Error: %s", TTF_GetError());
        m_isRunning = false;
        return;
    }
    m_pTextFont = TTF_OpenFont("../../assets/font/VonwaonBitmap-16px.ttf", 32);
    if(m_pTextFont == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"TTF_OpenFont Error: %s", TTF_GetError());
        m_isRunning = false;
        return;
    }

    //切换场景
    changeScene(new SceneTitle());
}

void Game::clean()
{
    if(m_pCurScene != nullptr){
        m_pCurScene->clean();
        delete m_pCurScene;
    }

    if(m_nearStars.m_pTexture != nullptr){
        SDL_DestroyTexture(m_nearStars.m_pTexture);
    }

    if(m_farStars.m_pTexture != nullptr){
        SDL_DestroyTexture(m_farStars.m_pTexture);
    }

    IMG_Quit();

    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);

    //清理字体
    if(m_pTitleFont != nullptr){
        TTF_CloseFont(m_pTitleFont);
        m_pTitleFont = nullptr;
    }
    if(m_pTextFont != nullptr){
        TTF_CloseFont(m_pTextFont);
        m_pTextFont = nullptr;
    }
    TTF_Quit();

    //清理并退出
    SDL_Quit();
}


SDL_Renderer* Game::getRenderer() const
{
    return m_pRenderer;
}

SDL_Window* Game::getWindow() const
{
    return m_pWindow;
}

bool Game::isRunning() const
{
    return m_isRunning;
}

int Game::getWindowWidth()
{
    return sm_nWindowWidth;
}

int Game::getWindowHeight()
{
    return sm_nWindowHeight;
}

void Game::stop()
{
    m_isRunning = false;
}


void Game::renderText(const std::string& text, float posY, bool isTitle)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* pTextSurface = nullptr;
    if(isTitle){
        pTextSurface = TTF_RenderUTF8_Solid(m_pTitleFont, text.c_str(), color);
    }
    else{
        pTextSurface = TTF_RenderUTF8_Solid(m_pTextFont, text.c_str(), color);
    }
    SDL_Texture* pTextTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTextSurface);
    if(pTextTexture == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        return;
    }

    int y = static_cast<int>((getWindowHeight() - pTextSurface->h) * posY);
    //SDL_Log("y: %d, windowHeight: %d, pTextSurface->h: %d", y, getWindowHeight(), pTextSurface->h );
    SDL_Rect dstRect = {static_cast<int>(getWindowWidth() / 2.0f - pTextSurface->w / 2.0f), y, pTextSurface->w, pTextSurface->h};
    SDL_RenderCopy(m_pRenderer, pTextTexture, nullptr, &dstRect);
    SDL_DestroyTexture(pTextTexture);
    SDL_FreeSurface(pTextSurface);
}