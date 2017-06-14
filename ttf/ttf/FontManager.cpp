//
//  FontManager.cpp
//  ttf
//
//  Created by Tamás Losonczi on 13/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include <SDL2_ttf/SDL_ttf.h>

#include "FontManager.hpp"
#include "SdlException.hpp"

std::vector<Font*> FontManager::fonts;

void FontManager::init() {
    
    if (TTF_Init() == -1) {
        throw SdlException(TTF_GetError());
    }
    
    fonts.push_back(new Font("Pixeled.ttf", 16));
}

void FontManager::destroy() {
    
    for (auto font : fonts) {
        delete font;
    }
    
    if (TTF_WasInit()) {
        TTF_Quit();
    }
    
}

Font *FontManager::getFont() {
    //add more fonts
    for (auto font : fonts) {
        return font;
    }
    return nullptr;
}
