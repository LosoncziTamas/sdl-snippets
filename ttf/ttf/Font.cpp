//
//  Font.cpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include "Font.hpp"
#include "SdlException.hpp"

Font::Font(const char* path, int size) : path(path), size(size), sdlFont(TTF_OpenFont(path, size)) {
    if (!sdlFont) {
        throw SdlException(TTF_GetError());
    }
}

Font::~Font() {
    if (sdlFont) {
        TTF_CloseFont(sdlFont);
    }
}