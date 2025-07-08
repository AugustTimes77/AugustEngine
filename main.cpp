#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

#include <SDL.h> // Add SDL2 header
#include <SDL_opengl.h> // Add SDL2 OpenGL header

#include "Utils.h"

#define numVAOs 1
#define numVBOs 2

using namespace std;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];


// build stack
stack<glm::mat4> mvStack;

// allocate variables used in display() fucntion so that they won't be needed to allocated during rendering
GLuint mvLoc, pLoc, tfLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

// set up cube vertices
void setupVertices(void) {
    float cubePositions[108] = {
        -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
    };

    float pyramidPositions[54] =
    { 
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // front face
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // right face
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // back face
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left face
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // base – left front
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // base – right back
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // activate the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW); // fill the buffer with data

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init(SDL_Window* window, SDL_GLContext context) {
    renderingProgram = Utils::createShaderProgram("./shaders/vertShader.glsl", "./shaders/fragShader.glsl");
    if (renderingProgram == 0) {
        std::cout << "Error creating shader program!" << std::endl;
        exit(EXIT_FAILURE);
    }
    cameraX = 0.0f; cameraY = 2.0f; cameraZ = 15.0f;
    cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f;
    pyrLocX = 0.0f; pyrLocY = 2.0f; pyrLocZ = 0.0f;
    setupVertices();

    glEnable(GL_DEPTH_TEST);
}

void display(SDL_Window* window, double currentTime) {
    GLenum err = glGetError();

    timeFactor = (float)currentTime;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram, "v_matrix"); // gets the lcoation of the uniforms
    pLoc = glGetUniformLocation(renderingProgram, "p_matrix"); // in the shader program

    // Use SDL_GL_GetDrawableSize for framebuffer size
    int drawableWidth, drawableHeight;
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
    width = drawableWidth; // Update global width
    height = drawableHeight; // Update global height

    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.4472f, aspect, 0.1f, 1000.0f);

    tfLoc = glGetUniformLocation(renderingProgram, "tf"); // Get the location of the "tf" uniform
    glUniform1f(tfLoc, timeFactor); // Pass the current time to the shader

    // the view matrix is computed once and used for both objects
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMat);

    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ---------------------- pyramid == sun --------------------------------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f)); // sun rotation
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
    mvStack.pop(); // remove the sun’s axial rotation from the stack

    //----------------------- cube == planet ---------------------------------------------
    mvStack.push(mvStack.top());
    mvStack.top() *=
    glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime)*4.0, 0.0f, cos((float)currentTime)*4.0));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
    // planet rotation
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36); // draw the planet
    mvStack.pop(); // remove the planet’s axial rotation from the stack
    //----------------------- smaller cube == moon -----------------------------------
    mvStack.push(mvStack.top());
    mvStack.top() *=
    glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime)*2.0,
    cos((float)currentTime)*2.0));
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0));
    // moon rotation
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // make the moon smaller
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36); // draw the moon

    // remove moon scale/rotation/position, planet position, sun position, and view matrices from stack
    mvStack.pop(); mvStack.pop(); mvStack.pop(); mvStack.pop();
}

// Function prototypes for SDL2
void init(SDL_Window* window, SDL_GLContext context);
void display(SDL_Window* window, double currentTime);

int main(int argc, char* argv[]) {
    // Explicitly set SDL to use the X11 video driver
    // SDL_SetHint(SDL_HINT_VIDEODRIVER, "x11");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create SDL Window
    SDL_Window* window = SDL_CreateWindow(
        "Chapter2 - program1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << "\n";
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create OpenGL context
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << "\n";
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Set swap interval
    SDL_GL_SetSwapInterval(1);

    init(window, context); // Pass window and context to init

    // Main loop
    SDL_Event event;
    bool quit = false;
    Uint64 lastTime = SDL_GetPerformanceCounter();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // Handle window resize if needed
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(window, &width, &height);
                glViewport(0, 0, width, height);
            }
        }

        Uint64 currentTime = SDL_GetPerformanceCounter();
        double deltaTime = (double)(currentTime - lastTime) / (double)SDL_GetPerformanceFrequency();
        lastTime = currentTime;

        display(window, SDL_GetTicks() / 1000.0); // Pass current time in seconds
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(EXIT_SUCCESS);
}
