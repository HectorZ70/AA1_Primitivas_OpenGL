#include "Utils.h"

#include <fstream>
#include <iostream>
#include <vector>

void ResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string LoadFile(const std::string& filePath)
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
    std::string code = LoadFile(path);
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
    std::string code = LoadFile(path);
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