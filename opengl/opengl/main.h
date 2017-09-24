#ifndef main_h
#define main_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

typedef GLuint (APIENTRY * GLCREATESHADERFN)( GLenum type );
extern GLCREATESHADERFN glCreateShader;


#endif
