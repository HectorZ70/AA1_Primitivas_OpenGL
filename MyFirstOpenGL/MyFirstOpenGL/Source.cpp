#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Pyramid.h"
#include "Square.h"
#include "Orthohedro.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct ShaderProgram
{
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string Load_File(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string content, line;

    if (!file.is_open()) {
        std::cerr << "Error abriendo archivo: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(file, line))
        content += line + "\n";

    return content;
}

// Vertex Shader
GLuint LoadVertexShader(const std::string& path)
{
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    std::string code = Load_File(path);
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Vertex shader error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

// Fragment Shader
GLuint LoadFragmentShader(const std::string& path)
{
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string code = Load_File(path);
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Fragment shader error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

GLuint CreateProgram(const ShaderProgram& shaders)
{
    GLuint program = glCreateProgram();

    if (shaders.vertexShader)
        glAttachShader(program, shaders.vertexShader);

    if (shaders.fragmentShader)
        glAttachShader(program, shaders.fragmentShader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetProgramInfoLog(program, len, nullptr, log.data());
        std::cerr << "Link error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return program;
}

int main() {

    glfwInit();

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Practica OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, Resize_Window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // Carga y compila shaders
    ShaderProgram shaders;
    shaders.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
    shaders.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

    GLuint program = CreateProgram(shaders);
    glUseProgram(program);

    // Localiza uniforms
    GLint offsetLocation = glGetUniformLocation(program, "offset");
    GLint timeLocation = glGetUniformLocation(program, "time");
    GLint objectTypeLocation = glGetUniformLocation(program, "objectType");

    // Creación de objetos
    Pyramid pyramid;
    Square cube;
    Orthohedro orthohedro;

    // Estados
    bool paused = false;
    bool wireframe = true;
    bool showCube = true;
    bool showOrtho = true;
    bool showPyramid = true;

    float speed = 1.0f;

    bool keyPressed[1024] = { false };

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float accumulatedTime = 0.0f;
    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        auto handleKey = [&](int key, auto action) {
            if (glfwGetKey(window, key) == GLFW_PRESS) {
                if (!keyPressed[key]) {
                    action();
                    keyPressed[key] = true;
                }
            }
            else {
                keyPressed[key] = false;
            }
            };

        // Pausa
        handleKey(GLFW_KEY_SPACE, [&]() {
            paused = !paused;
            });

        // Velocidad
        handleKey(GLFW_KEY_M, [&]() {
            if (!paused) speed *= 1.1f;
            });

        handleKey(GLFW_KEY_N, [&]() {
            if (!paused) speed *= 0.9f;
            });

        // Wireframe
        handleKey(GLFW_KEY_1, [&]() {
            if (!paused) {
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            }
            });

        // Renderización
        handleKey(GLFW_KEY_2, [&]() {
            if (!paused) showCube = !showCube;
            });

        handleKey(GLFW_KEY_3, [&]() {
            if (!paused) showOrtho = !showOrtho;
            });

        handleKey(GLFW_KEY_4, [&]() {
            if (!paused) showPyramid = !showPyramid;
            });

        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime;
        lastTime = currentTime;

        if (!paused)
            accumulatedTime += delta * speed;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniform1f(timeLocation, accumulatedTime);

        if (showPyramid) {
            glUniform1i(objectTypeLocation, 0);
            pyramid.Draw(offsetLocation, glm::vec2(0.6f, 0.0f));
        }

        if (showCube) {
            glUniform1i(objectTypeLocation, 1);
            cube.Draw(offsetLocation, glm::vec2(-0.6f, 0.0f));
        }

        if (showOrtho) {
            glUniform1i(objectTypeLocation, 2);
            orthohedro.Draw(offsetLocation, glm::vec2(0.0f, 0.0f));
        }

        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glfwTerminate();

    return 0;
}