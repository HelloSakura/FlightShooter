#include "SceneTitle.h"
#include "Game.h"
#include "SceneMain.h"

SceneTitle::SceneTitle() 
{
    init();
}


SceneTitle::~SceneTitle()
{

}

void SceneTitle::init()
{
    m_pBgm = Mix_LoadMUS("../../assets/music/06_Battle_in_Space_Intro.ogg");
    if(m_pBgm == nullptr){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Mix_LoadMUS Error: %s", Mix_GetError());
        return;
    }
    Mix_PlayMusic(m_pBgm, -1);
}

void SceneTitle::clean()
{
    if(m_pBgm != nullptr){
        Mix_FreeMusic(m_pBgm);
        m_pBgm = nullptr;
    }
}


void SceneTitle::update(float deltaTime)
{
}

void SceneTitle::render()
{
    //渲染标题文字
    m_game.renderTextCenter("FlightShooter", 0.4f, true);
    //渲染文本文字
    m_game.renderTextCenter("Press J to Start", 0.7f, false);
}



void SceneTitle::handleEvents(SDL_Event* pEvent)
{
    //SDL_Log("SceneTitle::handleEvents: %d", pEvent->type);
    //SDL_Log("SceneTitle::handleEvents: %d", pEvent->key.keysym.scancode);
    if(pEvent->type == SDL_KEYDOWN && pEvent->key.keysym.scancode == SDL_SCANCODE_J){
        m_game.changeScene(new SceneMain());
    }
}