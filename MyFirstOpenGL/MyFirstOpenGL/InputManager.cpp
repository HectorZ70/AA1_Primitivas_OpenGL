#include "InputManager.h"



#include "TimeManager.h"

const float SPEED_UP_FACTOR = 1.1f;
const float SPEED_DOWN_FACTOR = 0.9f;

InputManager::InputManager()
    :
    wireframe(true),
    showCube(true),
    showPyramid(true),
    showOrtho(true)
{
    for (int i = 0; i < 1024; i++)
    {
        keyPressed[i] = false;
    }
}

void InputManager::Update(
    GLFWwindow* window,
    TimeManager& time
)
{
    // =========================================
    // SPACE -> PAUSE
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_SPACE])
    {
        time.SetPaused(!time.IsPaused());

        keyPressed[GLFW_KEY_SPACE] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_SPACE] = false;
    }

    // =========================================
    // M -> SPEED UP
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_M])
    {
        time.SetTimeScale(
            time.GetTimeScale() * SPEED_UP_FACTOR
        );

        keyPressed[GLFW_KEY_M] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_M)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_M] = false;
    }

    // =========================================
    // N -> SPEED DOWN
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_N])
    {
        time.SetTimeScale(
            time.GetTimeScale() * SPEED_DOWN_FACTOR
        );

        keyPressed[GLFW_KEY_N] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_N)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_N] = false;
    }

    // =========================================
    // 1 -> WIREFRAME
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_1])
    {
        wireframe = !wireframe;

        if (wireframe)
        {
            glPolygonMode(
                GL_FRONT_AND_BACK,
                GL_LINE
            );
        }
        else
        {
            glPolygonMode(
                GL_FRONT_AND_BACK,
                GL_FILL
            );
        }

        keyPressed[GLFW_KEY_1] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_1)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_1] = false;
    }

    // =========================================
    // 2 -> CUBE
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_2])
    {
        showCube = !showCube;

        keyPressed[GLFW_KEY_2] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_2)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_2] = false;
    }

    // =========================================
    // 3 -> ORTHO
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_3])
    {
        showOrtho = !showOrtho;

        keyPressed[GLFW_KEY_3] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_3)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_3] = false;
    }

    // =========================================
    // 4 -> PYRAMID
    // =========================================

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS
        && !keyPressed[GLFW_KEY_4])
    {
        showPyramid = !showPyramid;

        keyPressed[GLFW_KEY_4] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_4)
        == GLFW_RELEASE)
    {
        keyPressed[GLFW_KEY_4] = false;
    }
}

bool InputManager::ShowCube() const
{
    return showCube;
}

bool InputManager::ShowPyramid() const
{
    return showPyramid;
}

bool InputManager::ShowOrtho() const
{
    return showOrtho;
}

bool InputManager::IsWireframe() const
{
    return wireframe;
}