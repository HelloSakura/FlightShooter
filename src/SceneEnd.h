/**
 * @file SceneEnd.h
 * @author Lucida
 * @date 2025-12-15
 * @brief 结束场景类
 */

#pragma once

#include "Scene.h"
#include<string>

class SceneEnd : public Scene{
public:
    SceneEnd();
    ~SceneEnd();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* pEvent) override;

private:
    void renderInputPage();
    void renderRecordPage();
    void removeLastUTF8Char(std::string& str);
    void updateBlinkTimer(float deltaTime);

private:
    std::string m_strName;
    bool m_bIsTyping = true;
    float m_fBlinkTimer = 0.0f;
    bool m_bIsBlinking = true;
};