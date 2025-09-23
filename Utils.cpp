#include "Utils.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

string readShaderSource(const char *filePath) {
    ifstream fileStream(filePath, ios::in);
    if(!fileStream.is_open()) {
        cout << "Failed to open file: " << filePath << endl;
        return "";
    }
    stringstream sstr;
    sstr << fileStream.rdbuf();
    return sstr.str();
}


void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log:" << log << endl;
        free(log);
    }
}

void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log:" << log << endl;
        free(log);
    }
}

namespace Utils {

    bool checkGLError(const char* operation) {
        bool foundError = false;
        int glErr = glGetError();
        while (glErr != GL_NO_ERROR) {
            cout << "OpenGL Error in " << operation << ": " << glErr << endl;
            foundError = true;
            glErr = glGetError();
        }
        return foundError;
    }

    GLuint loadTexture(const char *texImagePath) {
        GLuint textureID;
        textureID = SOIL_load_OGL_texture(texImagePath,
            SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if (textureID == 0) cout << "could not find texture file: " << texImagePath << endl;
        return textureID;
    }

    GLuint createShaderProgram(){
        GLint vertCompiled;
        GLint fragCompiled;
        GLint linked;

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

        string vertShaderStr = readShaderSource("../shaders/vertShader.glsl");
        string fragShaderStr = readShaderSource("../shaders/fragShader.glsl");

        if(vertShaderStr.empty() || fragShaderStr.empty()) {
            cout << "Failed to load shader files" << endl;
            return 0;
        }

        const char *vertShaderSrc = vertShaderStr.c_str();
        const char *fragShaderSrc = fragShaderStr.c_str();

        glShaderSource(vShader, 1, &vertShaderSrc, NULL);
        glShaderSource(fShader, 1, &fragShaderSrc, NULL);

        glCompileShader(vShader);
        checkGLError("vertex shader compilation");
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1){
            cout << "vertex compilation failed" << endl;
            printShaderLog(vShader);
        }

        glCompileShader(fShader);
        checkGLError("fragment shader compilation");
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
        if (fragCompiled != 1){
            cout << "fragment compilation failed" << endl;
            printShaderLog(fShader);
        }

        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);

        glLinkProgram(vfProgram);
        checkGLError("shader program linking");
        glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
        if(linked != 1){
            cout << "linking failed" << endl;
            printProgramLog(vfProgram);
        }

        return vfProgram;
    }
}
