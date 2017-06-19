//
//  Renderer.cpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include "Renderer.hpp"
#include "SdlException.hpp"

Renderer::Renderer(SDL_Window &sdlWindow) {
    sdlRenderer = SDL_CreateRenderer(&sdlWindow, -1, 0);
    if (!sdlRenderer) {
        throw SdlException(SDL_GetError());
    }
}

void Renderer::drawText(int x, int y, const Texture &fontTexture) {
    SDL_Rect dstRect = {x, y, fontTexture.w, fontTexture.h};
    SDL_RenderCopy(sdlRenderer, fontTexture.sdlTexture, NULL, &dstRect);
}

Texture Renderer::createTexture(const Font *font, const char *text, const SDL_Color &color) {
    SDL_Surface *sdlSurface = TTF_RenderUTF8_Solid(font->sdlFont, text, color);
    if (!sdlSurface)
    {
        throw SdlException(TTF_GetError());
    }
    SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
    SDL_FreeSurface(sdlSurface);
    if (!sdlTexture) {
        throw SdlException(SDL_GetError());
    }
    int w, h;
    TTF_SizeUTF8(font->sdlFont, text, &w, &h);
    return Texture(*sdlTexture, w, h);
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 0);
    SDL_RenderClear(sdlRenderer);
}

void Renderer::update(){
    SDL_RenderPresent(sdlRenderer);
}


Renderer::~Renderer() {
    SDL_DestroyRenderer(sdlRenderer);
}