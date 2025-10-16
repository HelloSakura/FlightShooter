#include "Game.h"
#include "SDL_log.h"
#include <SDL.h>
#include "SceneMain.h"
#include "SDL_image.h"

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
    while(m_isRunning)
    {
        SDL_Event event;
        handleEvents(&event);
        update();
        render();
    }
}

void Game::handleEvents(SDL_Event* pEvent)
{
    //处理退出事件
    while(SDL_PollEvent(pEvent))
    {
        if(pEvent->type == SDL_QUIT)
        {
            m_isRunning = false;
            break;
        }
        //处理场景事件
        m_pCurScene->handleEvents(pEvent);
    }
}


void Game::update()
{
    m_pCurScene->update();
}


void Game::render()
{
    //清空渲染场景
    SDL_RenderClear(m_pRenderer);
    //绘制场景内容
    m_pCurScene->render();
    //更新渲染场景
    SDL_RenderPresent(m_pRenderer);
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

    //切换场景
   changeScene(new SceneMain());
}

void Game::clean()
{
    if(m_pCurScene != nullptr)
    {
        m_pCurScene->clean();
        delete m_pCurScene;
    }

    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    IMG_Quit();
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