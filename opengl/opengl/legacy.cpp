#include "glextension.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool initGl() {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ) {
        printf( "Error initializing OpenGL! %s\n", SDL_GetError() );
        return false;
    }
    
    return true;
}

int legacyMain(void) {
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window *window = SDL_CreateWindow("SDL OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    
    if(glContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_GL_LoadExtensions();
    
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
    
    initGl();
    
    bool quit = false;
    
    SDL_Event e;
    
    while(!quit) {
        while( SDL_PollEvent(&e) != 0 ) {
            if( e.type == SDL_QUIT ) {
                quit = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glTranslatef(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f);
        
        glBegin(GL_QUADS);
        glColor3f(1.f, 0.f, 0.f); glVertex2f(-50.f, -50.f);
        glColor3f(1.f, 1.f, 0.f); glVertex2f( 50.f, -50.f);
        glColor3f(0.f, 1.f, 0.f); glVertex2f( 50.f,  50.f);
        glColor3f(0.f, 0.f, 1.f); glVertex2f(-50.f,  50.f);
        glEnd();
        
        SDL_GL_SwapWindow(window);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}