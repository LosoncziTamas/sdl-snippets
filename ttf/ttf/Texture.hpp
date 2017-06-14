//
//  Texture.hpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <SDL2/SDL.h>

class Texture {
    
public:
    Texture(SDL_Texture &sdlTexture, int w, int h);
    ~Texture();
    SDL_Texture * sdlTexture;
    int w,h;
};


#endif /* Texture_hpp */
