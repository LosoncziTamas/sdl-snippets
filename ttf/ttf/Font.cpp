//
//  Font.cpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include "Font.hpp"
#include "SdlException.hpp"
#include <iostream>

Font::Font(const std::string &path, int size) : path(path), size(size), sdlFont(TTF_OpenFont(path.c_str(), size)) {
    if (!sdlFont) {
        throw SdlException(TTF_GetError());
    }
}

Font::Font(const Font& other) : path(other.path), size(other.size), sdlFont(TTF_OpenFont(path.c_str(), size)) {}

Font::Font(Font &&other) : path(other.path), size(other.size), sdlFont(other.sdlFont) {
    other.sdlFont = nullptr;
}

Font& Font::operator=(Font other) {
    swap(*this, other);
    return *this;
}

Font::~Font() {
    if (TTF_WasInit()) {
        TTF_CloseFont(sdlFont);
    }
}

void swap(Font& first, Font& second) {
    using std::swap;
    swap(first.path, second.path);
    swap(first.size, second.size);
    swap(first.sdlFont, second.sdlFont);
}