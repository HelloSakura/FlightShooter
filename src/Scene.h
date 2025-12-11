/**
 * @file Scene.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 场景类
 */

#pragma once
#include "SDL_events.h"
#include <SDL.h>

class Game;
class Scene{
public:
    Scene();
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(SDL_Event* pEvent) = 0;

protected:
    Game& m_game;
};
