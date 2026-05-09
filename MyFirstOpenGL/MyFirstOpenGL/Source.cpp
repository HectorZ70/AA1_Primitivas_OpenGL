#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Utils.h"
#include "Pyramid.h"
#include "Square.h"
#include "Orthohedro.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const float SPEED_UP_FACTOR = 1.1f;
const float SPEED_DOWN_FACTOR = 0.9f;

const glm::vec2 PYRAMID_OFFSET(0.6f, 0.0f);
const glm::vec2 CUBE_OFFSET(-0.6f, 0.0f);
const glm::vec2 ORTHO_OFFSET(0.0f, 0.0f);


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

    RenderManager renderer;

    renderer.Initialize(
        "MyFirstVertexShader.glsl",
        "MyFirstFragmentShader.glsl"
    );

    // Objetos
    Pyramid pyramidMesh;
    Square cubeMesh;
    Orthohedro orthoMesh;

    GameObject pyramid(&pyramidMesh, 0);
    GameObject cube(&cubeMesh, 1);
    GameObject ortho(&orthoMesh, 2);

    InputManager input;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    TimeManager time;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        input.Update(window, time);

        // Actualizar tiempo
        time.Update();

        // =====================================================
        // PIRAMIDE
        // =====================================================

        pyramid.SetRotation(glm::vec3(
            time.GetTime(),
            time.GetTime(),
            0.0f
        ));

        pyramid.SetPosition(glm::vec3(
            0.6f,
            sin(time.GetTime()) * 0.75f,
            0.0f
        ));

        pyramid.SetScale(glm::vec3(0.5f));

        // =====================================================
        // CUBO
        // =====================================================

        cube.SetRotation(glm::vec3(
            0.0f,
            time.GetTime() * 2.0f,
            0.0f
        ));

        cube.SetPosition(glm::vec3(
            -0.6f,
            sin(time.GetTime()) * 0.75f,
            0.0f
        ));

        cube.SetScale(glm::vec3(0.5f));

        // =====================================================
        // ORTHOHEDRO
        // =====================================================

        float t =
            (sin(time.GetTime()) + 1.0f) * 0.5f;

        glm::vec3 orthoScale(1.0f, 0.5f, 0.3f);

        glm::vec3 cubeScale(0.5f);

        glm::vec3 finalScale =
            glm::mix(orthoScale, cubeScale, t);

        ortho.SetRotation(glm::vec3(
            0.0f,
            0.0f,
            time.GetTime() * 2.0f
        ));

        ortho.SetPosition(glm::vec3(
            0.0f,
            0.0f,
            0.0f
        ));

        ortho.SetScale(finalScale);

        // =====================================================
        // RENDER
        // =====================================================

        renderer.Clear();

        if (input.ShowPyramid())
            renderer.Render(
                pyramid,
                time.GetTime()
            );

        if (input.ShowCube())
            renderer.Render(
                cube,
                time.GetTime()
            );

        if (input.ShowOrtho())
            renderer.Render(
                ortho,
                time.GetTime()
            );

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}