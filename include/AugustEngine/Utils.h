#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <soil2/SOIL2.h>

namespace Utils {
    GLuint loadTexture(const char *texImagePath);
    GLuint createShaderProgram(); // Load shaders from files
    GLuint createShaderProgram(const char *vp, const char *fp);

    GLuint createShaderProgram(const char *vp, const char *gp, const char *fp);

    GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, const char *fp);

    GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES,
                                const char *gp, const char *fp);
    
    bool checkGLError(const char* operation = "OpenGL operation");
}


#endif
