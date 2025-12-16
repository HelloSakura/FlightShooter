/**
 * @file SceneTitle.h
 * @author Lucida
 * @date 2025-12-11
 * @brief 标题场景类
 */


#pragma once

#include "Scene.h"
#include<SDL_mixer.h>
 

class SceneTitle : public Scene{
public:
    SceneTitle();
    ~SceneTitle();

    virtual void init() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvents(SDL_Event* pEvent) override;

private:
    Mix_Music* m_pBgm = nullptr;

};