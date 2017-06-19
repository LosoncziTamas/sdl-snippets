//
//  Renderer.hpp
//  ttf
//
//  Created by Tamás Losonczi on 14/06/17.
//  Copyright © 2017 Tamás Losonczi. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "Font.hpp"
#include "Texture.hpp"

class Renderer {
    
public:
    Renderer(SDL_Window &sdlWindow);
    ~Renderer();
    
    void drawText(int x, int y, const Texture &fontTexture);
    void clear();
    void update();
    Texture createTexture(const Font *font, const char *text, const SDL_Color &color);
    
private:
    SDL_Renderer *sdlRenderer;
};
#endif
