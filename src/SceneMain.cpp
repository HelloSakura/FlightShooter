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
}

void SceneMain::update()
{
    keyboardControl();
}

void SceneMain::render()
{
    SDL_Rect rect = {static_cast<int>(m_player.m_fPosition.x), static_cast<int>(m_player.m_fPosition.y), m_player.m_nWidth, m_player.m_nHeight};
    SDL_RenderCopy(m_game.getRenderer(), m_player.m_pTexture, nullptr, &rect);
}

void SceneMain::clean()
{
    if(m_player.m_pTexture != nullptr)
    {
        SDL_DestroyTexture(m_player.m_pTexture);
        m_player.m_pTexture = nullptr;
    }
}

void SceneMain::handleEvents(SDL_Event* pEvent)
{

}

void SceneMain::keyboardControl()
{
    auto keyboardState = SDL_GetKeyboardState(nullptr);
    
    if(keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP])
    {
        m_player.m_fPosition.y -= 5.0f;
    }

    if (keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN])
    {
        m_player.m_fPosition.y += 5.0f;
    }

    if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT])
    {
        m_player.m_fPosition.x -= 5.0f;
    }

    if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT])
    {
        m_player.m_fPosition.x += 5.0f;
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
    
}