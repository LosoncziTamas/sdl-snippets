//
//  FontManager.cpp
//  ttf
//
//  Created by Tamás Losonczi on 13/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include <SDL2_ttf/SDL_ttf.h>
#include <assert.h>

#include "FontManager.hpp"
#include "SdlException.hpp"

std::vector<Font*> FontManager::fonts;
std::string FontManager::defaultFontPath = "pixeled.ttf";

void FontManager::init() {
    if (TTF_WasInit())  {
        return;
    }
    if (TTF_Init() == -1) {
        throw SdlException(TTF_GetError());
    }
}

void FontManager::destroy() {
    for (auto font : fonts) {
        delete font;
    }
    
    if (TTF_WasInit()) {
        TTF_Quit();
    }
}

Font *FontManager::getFontBySize(int size) {
    for (auto font : fonts) {
        if (font->size == size) {
            return font;
        }
    }
    Font *font = new Font(defaultFontPath, size);
    fonts.push_back(font);
    return font;
}

