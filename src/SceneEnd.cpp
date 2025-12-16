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
    if(m_bIsTyping){
        //处理输入页面
        if(pEvent->type == SDL_KEYDOWN){
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_RETURN){
                m_bIsTyping = false;
            }
            if(pEvent->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                if(!m_strName.empty()){
                    removeLastUTF8Char(m_strName);
                }
                SDL_StopTextInput();
            }
        }

        //处理文字输入
        if(pEvent->type == SDL_TEXTINPUT){
            m_strName += pEvent->text.text;
        }
    }
    else{
        //处理记录页面
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
    m_game.renderText("Your Score: " + std::to_string(m_game.getScore()), 0.2f, false);
    m_game.renderText("Game Over ", 0.4f, true);
    m_game.renderText("Please Enter your name: ", 0.6f, false);

    if(!m_strName.empty()){
        m_game.renderText(m_strName, 0.7f, false);
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