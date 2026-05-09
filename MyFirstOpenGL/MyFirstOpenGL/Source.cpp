#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Utils.h"
#include "Pyramid.h"
#include "Square.h"
#include "Orthohedro.h"
#include "GameObject.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const float SPEED_UP_FACTOR = 1.1f;
const float SPEED_DOWN_FACTOR = 0.9f;

const glm::vec2 PYRAMID_OFFSET(0.6f, 0.0f);
const glm::vec2 CUBE_OFFSET(-0.6f, 0.0f);
const glm::vec2 ORTHO_OFFSET(0.0f, 0.0f);


struct InputState 
{
    bool paused = false;
    bool wireframe = true;
    bool showCube = true;
    bool showOrtho = true;
    bool showPyramid = true;
    float speed = 1.0f;
    bool keyPressed[1024] = { false };
};


void ProcessInput(GLFWwindow* window, InputState& state)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_SPACE])
    {
        state.paused = !state.paused;
        state.keyPressed[GLFW_KEY_SPACE] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_SPACE] = false;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_M])
    {
        if (!state.paused) state.speed *= SPEED_UP_FACTOR;
        state.keyPressed[GLFW_KEY_M] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_M] = false;

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_N])
    {
        if (!state.paused) state.speed *= SPEED_DOWN_FACTOR;
        state.keyPressed[GLFW_KEY_N] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_N] = false;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_1])
    {
        if (!state.paused)
        {
            state.wireframe = !state.wireframe;
            if (state.wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        state.keyPressed[GLFW_KEY_1] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_1] = false;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_2])
    {
        if (!state.paused) state.showCube = !state.showCube;
        state.keyPressed[GLFW_KEY_2] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_2] = false;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_3])
    {
        if (!state.paused) state.showOrtho = !state.showOrtho;
        state.keyPressed[GLFW_KEY_3] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_3] = false;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !state.keyPressed[GLFW_KEY_4])
    {
        if (!state.paused) state.showPyramid = !state.showPyramid;
        state.keyPressed[GLFW_KEY_4] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE)
        state.keyPressed[GLFW_KEY_4] = false;
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Practica OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindow);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // Shaders
    ShaderProgram shaders;
    shaders.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
    shaders.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

    GLuint program = CreateProgram(shaders);
    glUseProgram(program);

    GLint offsetLocation = glGetUniformLocation(program, "offset");
    GLint timeLocation = glGetUniformLocation(program, "time");
    GLint objectTypeLocation = glGetUniformLocation(program, "objectType");

    // Objetos
    Pyramid pyramidMesh;
    Square cubeMesh;
    Orthohedro orthoMesh;

    GameObject pyramid(&pyramidMesh, 0);
    GameObject cube(&cubeMesh, 1);
    GameObject ortho(&orthoMesh, 2);

    InputState input;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float accumulatedTime = 0.0f;
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ProcessInput(window, input);

        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime;
        lastTime = currentTime;

        if (!input.paused)
            accumulatedTime += delta * input.speed;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniform1f(timeLocation, accumulatedTime);

        if (input.showPyramid) 
        {
            glUniform1i(objectTypeLocation, 0);
            pyramid.Draw();
        }

        if (input.showCube) 
        {
            glUniform1i(objectTypeLocation, 1);
            cube.Draw();
        }

        if (input.showOrtho) 
        {
            glUniform1i(objectTypeLocation, 2);
            ortho.Draw();
        }

        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}