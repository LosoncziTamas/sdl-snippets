//
//  Font.hpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#ifndef Font_hpp
#define Font_hpp

#include <SDL2_ttf/SDL_ttf.h>

class Font {
    
public:
    const char* path;
    int size;
    TTF_Font *sdlFont;
    
    Font(const char* path, int size);
    ~Font();
};

#endif /* Font_hpp */
