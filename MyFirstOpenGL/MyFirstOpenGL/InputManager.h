#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TimeManager;

class InputManager
{
private:

    bool keyPressed[1024];

    bool wireframe;

    bool showCube;
    bool showPyramid;
    bool showOrtho;

public:

    InputManager();

    void Update(
        GLFWwindow* window,
        TimeManager& time
    );

    bool ShowCube() const;

    bool ShowPyramid() const;

    bool ShowOrtho() const;

    bool IsWireframe() const;
};

