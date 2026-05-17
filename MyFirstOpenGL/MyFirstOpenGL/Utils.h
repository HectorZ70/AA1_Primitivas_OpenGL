#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Model.h"

struct ShaderProgram
{
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
};

void        ResizeWindow(GLFWwindow* window, int width, int height);
std::string LoadFile(const std::string& filePath);
GLuint      LoadVertexShader(const std::string& path);
GLuint      LoadFragmentShader(const std::string& path);
GLuint      CreateProgram(const ShaderProgram& shaders);

Model  LoadOBJModel(const std::string& filePath);
GLuint LoadTexture(const std::string& filePath);
