#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct Text_Prop
{
    const char *text;
    TTF_Font *font;
    int x;
    int y;
};

SDL_Texture *create_font_texture(SDL_Renderer *renderer, Text_Prop *properties)
{
    SDL_Color black = {0, 0, 0, 0};
    
    SDL_Surface *text_surface = TTF_RenderUTF8_Solid(properties->font, properties->text, black);
    if (!text_surface)
    {
        printf("TTF_RenderUTF8_Solid: %s\n", TTF_GetError());
        exit(2);
    }
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    
    return text_texture;
}

void render_font_texture(SDL_Renderer *renderer, SDL_Texture *text_texture, Text_Prop *properties)
{
    int w, h;
    TTF_SizeUTF8(properties->font, properties->text, &w, &h);
    SDL_Rect text_dst = {properties->x, properties->y, w, h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_dst);
}

int main(void) {

    SDL_Init(SDL_INIT_VIDEO);
    
    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    
    TTF_Font *font = TTF_OpenFont("Pixeled.ttf", 16);
    
    if (!font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(2);
    }
    
    SDL_Window *window = SDL_CreateWindow("ttf",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                0);
    SDL_bool running = SDL_TRUE;
    
    Text_Prop prop = {"TTF RENDERING", font, 128, 128};
    SDL_Texture *font_texture = create_font_texture(renderer, &prop);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    render_font_texture(renderer, font_texture, &prop);
    SDL_RenderPresent(renderer);
    
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = SDL_FALSE;
                } break;
            }
        }
    }
    
    SDL_DestroyTexture(font_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}

