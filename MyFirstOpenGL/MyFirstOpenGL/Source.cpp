#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <cmath>

#include "Utils.h"
#include "Primitive.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Camera.h"

const int   WINDOW_WIDTH = 640;
const int   WINDOW_HEIGHT = 480;
const float ASPECT_RATIO =
static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

const glm::vec3 PYRAMID_OFFSET(0.6f, 0.0f, 0.0f);
const glm::vec3 CUBE_OFFSET(-0.6f, 0.0f, 0.0f);
const glm::vec3 ORTHO_OFFSET(0.0f, 0.0f, 0.0f);

// Calcula MVP fuera del render — main.cpp orquesta
glm::mat4 ComputeMVP(const GameObject& obj, const Camera& cam)
{
    return cam.GetProjectionMatrix()
        * cam.GetViewMatrix()
        * obj.GetTransform().GetModelMatrix();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        "Practica OpenGL", NULL, NULL
    );
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

    Primitive pyramidMesh;
    pyramidMesh.SetVerticesAndVariables(
        { -0.5f,0.0f,-0.5f,  0.5f,0.0f,-0.5f,
           0.5f,0.0f, 0.5f, -0.5f,0.0f, 0.5f,
           0.0f,0.8f, 0.0f },
        { 0,1,2, 2,3,0,
          0,1,4, 1,2,4, 2,3,4, 3,0,4 }
    );
    Primitive cubeMesh;
    cubeMesh.SetVerticesAndVariables(
        { -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
          -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
           0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f },
        { 0,1,2, 2,3,0,   5,4,7, 7,6,5,
          4,0,3, 3,7,4,   1,5,6, 6,2,1,
          3,2,6, 6,7,3,   4,5,1, 1,0,4 }
    );
    Primitive orthoMesh;
    orthoMesh.SetVerticesAndVariables(
        { -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
          -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
           0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f },
        { 0,1,2, 2,3,0,   5,4,7, 7,6,5,
          4,0,3, 3,7,4,   1,5,6, 6,2,1,
          3,2,6, 6,7,3,   4,5,1, 1,0,4 }
    );

    GameObject pyramid(&pyramidMesh, 0);
    GameObject cube(&cubeMesh, 1);
    GameObject ortho(&orthoMesh, 2);

    Camera camera(70.0f, ASPECT_RATIO);
    camera.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    InputManager input;
    SceneManager sceneManager;
    TimeManager  time;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        input.Update(window, time, sceneManager);
        time.Update();

        const float t = time.GetTime();

        // Pyramid: rota XY, rebota en Y
        pyramid.GetTransform().SetPosition(
            PYRAMID_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
        pyramid.GetTransform().SetRotation(glm::vec3(t, t, 0.0f));
        pyramid.GetTransform().SetScale(glm::vec3(0.5f));

        // Cube: rota Y, rebota en Y
        cube.GetTransform().SetPosition(
            CUBE_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
        cube.GetTransform().SetRotation(glm::vec3(0.0f, t * 2.0f, 0.0f));
        cube.GetTransform().SetScale(glm::vec3(0.5f));

        // Ortho: interpola forma, rota Z
        float blend = (sin(t) + 1.0f) * 0.5f;
        ortho.GetTransform().SetPosition(ORTHO_OFFSET);
        ortho.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, t * 2.0f));
        ortho.GetTransform().SetScale(glm::mix(
            glm::vec3(1.0f, 0.5f, 0.3f),
            glm::vec3(0.5f),
            blend
        ));

        renderer.Clear();

        if (sceneManager.IsGameScene())
        {
            if (input.ShowPyramid() && pyramid.IsVisible())
                renderer.Render(
                    *pyramid.GetPrimitive(),
                    ComputeMVP(pyramid, camera),
                    pyramid.GetObjectType(), t
                );

            if (input.ShowCube() && cube.IsVisible())
                renderer.Render(
                    *cube.GetPrimitive(),
                    ComputeMVP(cube, camera),
                    cube.GetObjectType(), t
                );

            if (input.ShowOrtho() && ortho.IsVisible())
                renderer.Render(
                    *ortho.GetPrimitive(),
                    ComputeMVP(ortho, camera),
                    ortho.GetObjectType(), t
                );
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}