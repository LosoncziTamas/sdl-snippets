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
#include <string>

class Font {
    
public:
    std::string path;
    int size;
    TTF_Font *sdlFont;
    
    Font(const std::string &path, int size);
    Font(const Font& other);
    Font(Font&& other);
    Font &operator=(Font other);
    ~Font();
    
    friend void swap(Font& first, Font& second);

};

#endif /* Font_hpp */
