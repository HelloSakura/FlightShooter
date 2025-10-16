/**
 * @file SceneMain.h
 * @author Lucida
 * @date 2025-10-15
 * @brief 主场景类
 */

#pragma once

#include "Scene.h"
#include "Object.h"

class Game;

class SceneMain : public Scene{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update() override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* pEvent) override;

private:
    Game& m_game;
    Player m_player;
};
