#include "glextension.hpp"

GLCREATESHADERFN glCreateShader = NULL;
GLCREATEPROGRAMFN glCreateProgram = NULL;
GLSHADERSOURCEFN glShaderSource = NULL;
GLCOMPILESHADERFN glCompileShader = NULL;
GLGETSHADERINFOLOGFN glGetShaderInfoLog = NULL;
GLATTACHSHADERFN glAttachShader = NULL;
GLLINKPROGRAMFN glLinkProgram = NULL;
GLGETPROGRAMINFOLOGFN glGetProgramInfoLog = NULL;
GLISPROGRAMFN glIsProgram = NULL;
GLISSHADERFN glIsShader = NULL;
GLGETATTACHEDSHADERSFN glGetAttachedShaders = NULL;
GLGETUNIFORMLOCATIONFN glGetUniformLocation = NULL;
GLUNIFORM1FFN glUniform1f = NULL;
GLUNIFORM1IFN glUniform1i = NULL;
GLDELETESHADERFN glDeleteShader = NULL;
GLDELETEPROGRAMFN glDeleteProgram = NULL;
GLUSEPROGRAMFN glUseProgram = NULL;
GLVERTEXATTRIBPOINTERFN glVertexAttribPointer = NULL;
GLVERTEXATTRIBDIVISORFN glVertexAttribDivisor = NULL;
GLENABLEVERTEXATTRIBARRAYFN glEnableVertexAttribArray = NULL;
GLDISABLEVERTEXATTRIBARRAYFN glDisableVertexAttribArray = NULL;
GLUNIFORM2FFN glUniform2f = NULL;
GLUNIFORM3FFN glUniform3f = NULL;
GLUNIFORM4FFN glUniform4f = NULL;
GLUNIFORMMATRIX3FVFN glUniformMatrix3fv = NULL;
GLUNIFORMMATRIX4FVFN glUniformMatrix4fv = NULL;
GLUNIFORM1FVFN glUniform1fv = NULL;
GLUNIFORM2FVFN glUniform2fv = NULL;
GLUNIFORM3FVFN glUniform3fv = NULL;
GLUNIFORM3FVFN glUniform4fv = NULL;
GLGETSHADERIVFN glGetShaderiv = NULL;
GLPROGRAMPARAMETERIFN glProgramParameteri = NULL;
GLGETPROGRAMIVFN glGetProgramiv = NULL;
GLBINDATTRIBLOCATIONFN glBindAttribLocation = NULL;
GLGENFRAMEBUFFERSFN glGenFramebuffers = NULL;
GLBINDFRAMEBUFFERFN glBindFramebuffer = NULL;
GLBLITFRAMEBUFFERFN glBlitFramebuffer = NULL;
GLFRAMEBUFFERTEXTUREFN glFramebufferTexture = NULL;
GLFRAMEBUFFERTEXTURE2DFN glFramebufferTexture2D = NULL;
GLDELETEFRAMEBUFFERSFN glDeleteFramebuffers = NULL;
GLCHECKFRAMEBUFFERSTATUSFN glCheckFramebufferStatus = NULL;
GLGENBUFFERSFN glGenBuffers = NULL;
GLGENRENDERBUFFERSFN glGenRenderbuffers = NULL;
GLDELETEBUFFERSFN glDeleteBuffers = NULL;
GLDELETERENDERBUFFERSFN glDeleteRenderbuffers = NULL;
GLBINDBUFFERFN glBindBuffer = NULL;
GLBINDRENDERBUFFERFN glBindRenderbuffer = NULL;
GLBUFFERDATAFN glBufferData = NULL;
GLGETBUFFERSUBDATAFN glGetBufferSubData = NULL;
GLFRAMEBUFFERRENDERBUFFERFN glFramebufferRenderbuffer = NULL;
GLGETATTRIBLOCATIONFN glGetAttribLocation = NULL;
GLRENDERBUFFERSTORAGEFN glRenderbufferStorage = NULL;
GLRENDERBUFFERSTORAGEMULTISAMPLEFN glRenderbufferStorageMultisample = NULL;
GLDRAWBUFFERSFN glDrawBuffers = NULL;
GLGENERATEMIPMAPFN glGenerateMipmap = NULL;
GLDRAWELEMENTSINSTANCEDFN glDrawElementsInstanced = NULL;
GLPATCHPARAMETERIFN glPatchParameteri = NULL;
GLPATCHPARAMETERFVFN glPatchParameterfv = NULL;
GLVALIDATEPROGRAMFN glValidateProgram = NULL;
GLGENVERTEXARRAYSFN glGenVertexArrays = NULL;
GLBINDVERTEXARRAYFN glBindVertexArray = NULL;

GLBROKENEXTENSIONFN glBrokenExtension = NULL;

#define SDL_GL_LoadExtension(type, name) \
name = (type)SDL_GL_GetProcAddress(#name); \
if (name == NULL) { \
fprintf(stderr, "Failed to load function '%s', looking for function '%s'...\n", #name, #name"EXT"); \
name = (type)SDL_GL_GetProcAddress(#name"EXT"); \
} \
if (name == NULL) { \
fprintf(stderr, "Failed to load function '%s', looking for function '%s'...\n", #name"EXT", #name"ARB"); \
name = (type)SDL_GL_GetProcAddress(#name"ARB"); \
} \
if (name == NULL) { fprintf(stderr, "Completely failed to load OpenGL extension function '%s'. Use of this function will crash\n", #name); }

void SDL_GL_LoadExtensions() {
    
    /* Shaders */
    
    SDL_GL_LoadExtension(GLCREATEPROGRAMFN, glCreateProgram);
    SDL_GL_LoadExtension(GLLINKPROGRAMFN, glLinkProgram);
    SDL_GL_LoadExtension(GLDELETEPROGRAMFN, glDeleteProgram);
    SDL_GL_LoadExtension(GLGETPROGRAMINFOLOGFN, glGetProgramInfoLog);
    SDL_GL_LoadExtension(GLUSEPROGRAMFN, glUseProgram);
    SDL_GL_LoadExtension(GLGETPROGRAMIVFN, glGetProgramiv);
    SDL_GL_LoadExtension(GLPROGRAMPARAMETERIFN, glProgramParameteri);
    SDL_GL_LoadExtension(GLVALIDATEPROGRAMFN, glValidateProgram);
    
    SDL_GL_LoadExtension(GLCREATESHADERFN, glCreateShader);
    SDL_GL_LoadExtension(GLSHADERSOURCEFN, glShaderSource);
    SDL_GL_LoadExtension(GLCOMPILESHADERFN, glCompileShader);
    SDL_GL_LoadExtension(GLGETSHADERINFOLOGFN, glGetShaderInfoLog);
    SDL_GL_LoadExtension(GLATTACHSHADERFN, glAttachShader);
    SDL_GL_LoadExtension(GLDELETESHADERFN, glDeleteShader);
    SDL_GL_LoadExtension(GLGETSHADERIVFN, glGetShaderiv);
    SDL_GL_LoadExtension(GLISPROGRAMFN, glIsProgram);
    SDL_GL_LoadExtension(GLISSHADERFN, glIsShader);
    SDL_GL_LoadExtension(GLGETATTACHEDSHADERSFN, glGetAttachedShaders);
    
    SDL_GL_LoadExtension(GLGETUNIFORMLOCATIONFN, glGetUniformLocation);
    SDL_GL_LoadExtension(GLUNIFORM1FFN, glUniform1f);
    SDL_GL_LoadExtension(GLUNIFORM1IFN, glUniform1i);
    SDL_GL_LoadExtension(GLUNIFORM2FFN, glUniform2f);
    SDL_GL_LoadExtension(GLUNIFORM3FFN, glUniform3f);
    SDL_GL_LoadExtension(GLUNIFORM4FFN, glUniform4f);
    SDL_GL_LoadExtension(GLUNIFORM1FVFN, glUniform1fv);
    SDL_GL_LoadExtension(GLUNIFORM2FVFN, glUniform2fv);
    SDL_GL_LoadExtension(GLUNIFORM3FVFN, glUniform3fv);
    SDL_GL_LoadExtension(GLUNIFORM3FVFN, glUniform4fv);
    SDL_GL_LoadExtension(GLUNIFORMMATRIX3FVFN, glUniformMatrix3fv);
    SDL_GL_LoadExtension(GLUNIFORMMATRIX4FVFN, glUniformMatrix4fv);
    
    /* Attributes */
    
    SDL_GL_LoadExtension(GLGETATTRIBLOCATIONFN, glGetAttribLocation);
    SDL_GL_LoadExtension(GLVERTEXATTRIBPOINTERFN, glVertexAttribPointer);
    SDL_GL_LoadExtension(GLVERTEXATTRIBDIVISORFN, glVertexAttribDivisor);
    SDL_GL_LoadExtension(GLENABLEVERTEXATTRIBARRAYFN, glEnableVertexAttribArray);
    SDL_GL_LoadExtension(GLDISABLEVERTEXATTRIBARRAYFN, glDisableVertexAttribArray);
    SDL_GL_LoadExtension(GLBINDATTRIBLOCATIONFN, glBindAttribLocation);
    SDL_GL_LoadExtension(GLGENVERTEXARRAYSFN, glGenVertexArrays);
    SDL_GL_LoadExtension(GLBINDVERTEXARRAYFN, glBindVertexArray);
    
    /* Textures */
    
    SDL_GL_LoadExtension(GLGENERATEMIPMAPFN, glGenerateMipmap);
    
    /* Buffers */
    
    SDL_GL_LoadExtension(GLGENBUFFERSFN, glGenBuffers);
    SDL_GL_LoadExtension(GLBINDBUFFERFN, glBindBuffer);
    SDL_GL_LoadExtension(GLBUFFERDATAFN, glBufferData);
    SDL_GL_LoadExtension(GLGETBUFFERSUBDATAFN, glGetBufferSubData);
    SDL_GL_LoadExtension(GLDELETEBUFFERSFN, glDeleteBuffers);
    SDL_GL_LoadExtension(GLDRAWBUFFERSFN, glDrawBuffers);
    
    SDL_GL_LoadExtension(GLGENRENDERBUFFERSFN, glGenRenderbuffers);
    SDL_GL_LoadExtension(GLBINDRENDERBUFFERFN, glBindRenderbuffer);
    SDL_GL_LoadExtension(GLRENDERBUFFERSTORAGEFN, glRenderbufferStorage);
    SDL_GL_LoadExtension(GLRENDERBUFFERSTORAGEMULTISAMPLEFN, glRenderbufferStorageMultisample);
    SDL_GL_LoadExtension(GLDELETERENDERBUFFERSFN, glDeleteRenderbuffers);
    
    SDL_GL_LoadExtension(GLGENFRAMEBUFFERSFN, glGenFramebuffers);
    SDL_GL_LoadExtension(GLBINDFRAMEBUFFERFN, glBindFramebuffer);
    SDL_GL_LoadExtension(GLBLITFRAMEBUFFERFN, glBlitFramebuffer);
    SDL_GL_LoadExtension(GLFRAMEBUFFERTEXTUREFN, glFramebufferTexture);
    SDL_GL_LoadExtension(GLFRAMEBUFFERTEXTURE2DFN, glFramebufferTexture2D);
    SDL_GL_LoadExtension(GLDELETEFRAMEBUFFERSFN, glDeleteFramebuffers);
    SDL_GL_LoadExtension(GLCHECKFRAMEBUFFERSTATUSFN, glCheckFramebufferStatus);
    SDL_GL_LoadExtension(GLFRAMEBUFFERRENDERBUFFERFN, glFramebufferRenderbuffer);
    
    /* Tessellation */
    
    SDL_GL_LoadExtension(GLPATCHPARAMETERIFN, glPatchParameteri);
    SDL_GL_LoadExtension(GLPATCHPARAMETERFVFN, glPatchParameterfv);
    
    /* Misc */
    
    SDL_GL_LoadExtension(GLDRAWELEMENTSINSTANCEDFN, glDrawElementsInstanced);
    
}
