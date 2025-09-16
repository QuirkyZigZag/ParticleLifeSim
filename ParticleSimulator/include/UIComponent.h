#pragma once
#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "Entity.hpp"
#include "TextureManager.hpp"
#include <string>
#include"Components.hpp"
#include "SDL2/SDL_ttf.h"

class UIComponent : public Component{
private:


    std::string text;
    SDL_Color color;
    SDL_Texture* texture;
    SDL_Rect textRect;
    TTF_Font* font;


public:
    UIComponent(const std::string& text, const std::string& fontPath, int fontSize, SDL_Color color)
        : text(text), color(color), texture(nullptr), font(nullptr){
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font){
            std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        }
        updateTexture();
    }

    ~UIComponent(){
        if (texture) SDL_DestroyTexture(texture);
        if (font) TTF_CloseFont(font);
    }

    void setText(const std::string& newText){
        if (text != newText){
            text = newText;
            updateTexture();
        }
    }

    void updateTexture(){
        if (!font) return;
        if (texture) SDL_DestroyTexture(texture);

        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface){
            std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
            return;
        }

        texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
        if (!texture){
            std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        }
        textRect.w = surface->w;
        textRect.h = surface->h;
        SDL_FreeSurface(surface);
    }

    void init() override{
        updateTexture();
    }

    void update() override{
        auto& transform = entity->getComponent<TransformComponent>();
        textRect.x = static_cast<int>(transform.Position.x);
        textRect.y = static_cast<int>(transform.Position.y);
    }

    void draw() override{
        if (texture) {
            SDL_RenderCopy(Game::renderer, texture, nullptr, &textRect);
        }
    }
};

#endif



