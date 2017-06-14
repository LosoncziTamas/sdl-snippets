//
//  Texture.cpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#include "Texture.hpp"

Texture::Texture(SDL_Texture &sdlTexture, int w, int h) : sdlTexture(&sdlTexture), w(w), h(h) { }

Texture::~Texture(){
    SDL_DestroyTexture(sdlTexture);
}
