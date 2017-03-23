#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define AUDIO_BUFF_SIZE 1024
#define MIXER_CHANNELS 1

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    
    SDL_Window *window = SDL_CreateWindow("Sound",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_RESIZABLE);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                0);
    
    if (-1 == Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                           MIX_DEFAULT_FORMAT,
                           MIX_DEFAULT_CHANNELS,
                           AUDIO_BUFF_SIZE))
    {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }
    
    Mix_AllocateChannels(MIXER_CHANNELS);
    
    Mix_Music *music = Mix_LoadMUS("music.ogg");
    if (!music)
    {
        printf("Mix_LoadMUS: %s\n", Mix_GetError());
    }
    
    Mix_Chunk *sample = Mix_LoadWAV("jump.wav");
    if (!sample)
    {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    
    Mix_PlayMusic(music, -1);
    
    SDL_bool running = SDL_TRUE;
    
    SDL_RenderClear(renderer);
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
                case SDL_KEYDOWN:
                {
                    Mix_PlayChannel(0, sample, 0);
                } break;

            }
        }
    }
    
    Mix_FreeMusic(music);
    Mix_FreeChunk(sample);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
