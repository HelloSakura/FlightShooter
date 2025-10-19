/**
 * @file Game.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 游戏类 单例
 */

#pragma once

#include "Scene.h"
#include <SDL.h>
#include <SDL_events.h>

class Game{
public:
    static Game& getInstance()
    {
        static Game sm_instance;
        return sm_instance;
    }

    ~Game();

    void run();     //游戏主循环
    void update(float deltaTime);  //更新游戏
    void render();  //渲染游戏
    void handleEvents(SDL_Event* pEvent);  //处理事件
    void changeScene(Scene* pScene);  //切换场景
    void init();    //初始化游戏
    void clean();   //清理游戏
    void stop();
    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
    bool isRunning() const;

    static int getWindowWidth();
    static int getWindowHeight();
    


private:
    Game(); //私有化构造函数
    Game(const Game&) = delete;     //禁止拷贝构造
    Game& operator=(const Game&) = delete; //禁止赋值构造

private:
    bool m_isRunning = true;
    Scene* m_pCurScene = nullptr;
    SDL_Window* m_pWindow = nullptr;
    SDL_Renderer* m_pRenderer = nullptr;
    int m_nFPS = 60;
    Uint32 m_nFrameTime;
    float m_fDeltaTime = 0.0f;
    

private:
    static int sm_nWindowWidth;
    static int sm_nWindowHeight;
};
