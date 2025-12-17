/**
 * @file Game.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 游戏类 单例
 */

#pragma once

#include "Object.h"
#include "Scene.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include<SDL_ttf.h>
#include <string>

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
    void updateBackground(float deltaTime);  //更新背景卷轴
    void render();  //渲染游戏
    void renderBackground();  //渲染星空背景
    void handleEvents(SDL_Event* pEvent);  //处理事件
    void changeScene(Scene* pScene);  //切换场景
    void init();    //初始化游戏
    void clean();   //清理游戏
    void stop();
    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
    bool isRunning() const;

    //渲染工具函数
    SDL_Point renderTextCenter(const std::string& text, float posY, bool isTitle);
    void renderTextPos(const std::string& text, int posX, int posY, bool isTitle);


    static int getWindowWidth();
    static int getWindowHeight();
    
    void setScore(int score);
    int getScore() const;

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

    Background m_nearStars;
    Background m_farStars;
    TTF_Font* m_pTitleFont = nullptr;
    TTF_Font* m_pTextFont = nullptr;
    int m_nScore = 0;

private:
    static int sm_nWindowWidth;
    static int sm_nWindowHeight;
};
