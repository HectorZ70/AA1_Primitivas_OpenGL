#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Utils.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"

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

        // Objects

    Primitive pyramidMesh;
    pyramidMesh.SetVerticesAndVariables(
        std::vector<GLfloat>{
        -0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, -0.5f,
            0.5f, 0.0f, 0.5f,
            -0.5f, 0.0f, 0.5f,

            0.0f, 0.8f, 0.0f
    },

        std::vector<GLuint>{
        0, 1, 2,
            2, 3, 0,

            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
    }
    );
    Primitive cubeMesh;
    cubeMesh.SetVerticesAndVariables(
        std::vector<GLfloat>{
        -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f},

        std::vector<GLuint>{
        // Frontal
        0, 1, 2,
            2, 3, 0,

            // Back
            5, 4, 7,
            7, 6, 5,

            // Left
            4, 0, 3,
            3, 7, 4,

            // Right
            1, 5, 6,
            6, 2, 1,

            // Superior
            3, 2, 6,
            6, 7, 3,

            // Inferior
            4, 5, 1,
            1, 0, 4
    }
    );
    Primitive orthoMesh;
    orthoMesh.SetVerticesAndVariables(
        std::vector<GLfloat>{
        // Front
        -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            // Back
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f},

        std::vector<GLuint>{
            // Front
            0, 1, 2,
                2, 3, 0,

                // Back
                5, 4, 7,
                7, 6, 5,

                // Left
                4, 0, 3,
                3, 7, 4,

                // Right
                1, 5, 6,
                6, 2, 1,

                // Superior
                3, 2, 6,
                6, 7, 3,

                // Inferior
                4, 5, 1,
                1, 0, 4}
    );

    GameObject pyramid(&pyramidMesh, 0);
    GameObject cube(&cubeMesh, 1);
    GameObject ortho(&orthoMesh, 2);

    InputManager input;
    SceneManager sceneManager;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    TimeManager time;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        input.Update(window, time, sceneManager);

        time.Update();

        // Pyramid
        pyramid.SetRotation(glm::vec3(
            time.GetTime(),
            time.GetTime(),
            0.0f
        ));

        pyramid.SetPosition(glm::vec3(
            PYRAMID_OFFSET.x,
            sin(time.GetTime()) * 0.75f,
            PYRAMID_OFFSET.y
        ));

        pyramid.SetScale(glm::vec3(0.5f));

        // Cube
        cube.SetRotation(glm::vec3(
            0.0f,
            time.GetTime() * 2.0f,
            0.0f
        ));

        cube.SetPosition(glm::vec3(
            CUBE_OFFSET.x,
            sin(time.GetTime()) * 0.75f,
            CUBE_OFFSET.y
        ));

        cube.SetScale(glm::vec3(0.5f));

        // Ortho
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
            ORTHO_OFFSET.x,
            0.0f,
            ORTHO_OFFSET.y
        ));

        ortho.SetScale(finalScale);

        // Render
        renderer.Clear();

        if (sceneManager.IsGameScene())
        {
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
        }
        else if (sceneManager.IsEmptyScene())
        {
            // Escena completamente vacia
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}