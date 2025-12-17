#include "SceneEnd.h"
#include "Game.h"
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <string>
#include "SceneTitle.h"

SceneEnd::SceneEnd()
{
    
}

SceneEnd::~SceneEnd()
{

}


void SceneEnd::init()
{
    if(!SDL_IsTextInputActive()){
        SDL_StartTextInput();
    }

    if(!SDL_IsTextInputActive()){
        SDL_Log("SDL_IsTextInputActive is false");
    }
}

void SceneEnd::update(float deltaTime)
{
    updateBlinkTimer(deltaTime);
}

void SceneEnd::render()
{
    if(m_bIsTyping){
        renderInputPage();
    }
    else{
        renderRecordPage();
    }
}

void SceneEnd::clean()
{

}

void SceneEnd::handleEvents(SDL_Event* pEvent)
{
    SDL_Log("SceneEnd::handleEvents: %d", pEvent->type);
    if(m_bIsTyping){
        SDL_Log("SceneEnd::handleEvents: typing");
        //处理输入页面
        if(pEvent->type == SDL_KEYDOWN){
            SDL_Log("SceneEnd::handleEvents: keydown scancode: %d", pEvent->key.keysym.scancode);
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_RETURN){
                m_bIsTyping = false;
            }
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                if(!m_strName.empty()){
                    removeLastUTF8Char(m_strName);
                }
            }
        }

        //处理文字输入
        if(pEvent->type == SDL_TEXTINPUT){
            SDL_Log("SceneEnd::handleEvents: textinput text: %s", pEvent->text.text);
            m_strName += pEvent->text.text;
        }
    }
    else{
        if(SDL_IsTextInputActive()){
            SDL_StopTextInput();
        }
        //处理记录页面
        SDL_Log("SceneEnd::handleEvents: record");
        if(pEvent->type == SDL_KEYDOWN){
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_R){
                m_game.changeScene(new SceneTitle());
            }
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                m_game.stop();
            }
        }
    }
    
}

void SceneEnd::renderInputPage()
{
    m_game.renderTextCenter("Your Score: " + std::to_string(m_game.getScore()), 0.2f, false);
    m_game.renderTextCenter("Game Over ", 0.4f, true);
    m_game.renderTextCenter("Please Enter your name: ", 0.6f, false);

    if(!m_strName.empty()){
        SDL_Point pos = m_game.renderTextCenter(m_strName, 0.7f, false);
        if(m_bIsBlinking){
            m_game.renderTextPos("_", pos.x, pos.y, false);
        }
    }
    else{
        if(m_bIsBlinking){
            m_game.renderTextCenter("_", 0.7f, false);
        }
    }
}

void SceneEnd::renderRecordPage()
{

}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    if(str.empty()){
        return;
    }

    auto lastChar = str.back();
    if((lastChar & 0b10000000) == 0b10000000){  // 中文字符后续字节判断
        str.pop_back();
        while((str.back() & 0b11000000) != 0b11000000){  // 找到中文首字节
            str.pop_back();
        }
    }
    str.pop_back();
}

void SceneEnd::updateBlinkTimer(float deltaTime)
{
    m_fBlinkTimer += deltaTime;
    if(m_fBlinkTimer >= 0.5f){
        m_fBlinkTimer = 0.0f;
        m_bIsBlinking = !m_bIsBlinking;
    }
}