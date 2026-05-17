#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>

#include "Utils.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ModelGameObject.h"
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
const glm::vec3 TROLL_OFFSET(-0.6f, 0.0f, 0.0f);
const glm::vec3 ROCK_OFFSET(0.6f, 0.0f, 0.0f);

glm::mat4 ComputeMVP(const Transform& t, const Camera& cam)
{
    return cam.GetProjectionMatrix()
        * cam.GetViewMatrix()
        * t.GetModelMatrix();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        "Practica OpenGL", NULL, NULL);
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

    // --- Primitives ---
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

    // --- Models OBJ ---
    Model trollModel = LoadOBJModel("Assets/Modelos/troll.obj");
    Model rockModel = LoadOBJModel("Assets/Modelos/rock.obj");
    trollModel.SetTexture(LoadTexture("Assets/Texturas/troll.png"));
    rockModel.SetTexture(LoadTexture("Assets/Texturas/rock.png"));

    ModelGameObject troll(&trollModel);
    ModelGameObject rock(&rockModel);
    troll.GetTransform().SetPosition(TROLL_OFFSET);
    rock.GetTransform().SetPosition(ROCK_OFFSET);

    // --- Camera ---
    Camera camera(70.0f, ASPECT_RATIO);
    camera.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    InputManager input;
    SceneManager sceneManager;
    TimeManager  time;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        input.Update(window, time, sceneManager);
        time.Update();

        const float t = time.GetTime();

        // Pyramid
        pyramid.GetTransform().SetPosition(
            PYRAMID_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
        pyramid.GetTransform().SetRotation(glm::vec3(t, t, 0.0f));
        pyramid.GetTransform().SetScale(glm::vec3(0.5f));

        // Cube
        cube.GetTransform().SetPosition(
            CUBE_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
        cube.GetTransform().SetRotation(glm::vec3(0.0f, t * 2.0f, 0.0f));
        cube.GetTransform().SetScale(glm::vec3(0.5f));

        // Ortho
        float blend = (sin(t) + 1.0f) * 0.5f;
        ortho.GetTransform().SetPosition(ORTHO_OFFSET);
        ortho.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, t * 2.0f));
        ortho.GetTransform().SetScale(glm::mix(
            glm::vec3(1.0f, 0.5f, 0.3f),
            glm::vec3(0.5f), blend));

        renderer.Clear();

        if (sceneManager.IsGameScene())
        {
            // Primitives
            if (input.ShowPyramid())
                renderer.Render(*pyramid.GetPrimitive(),
                    ComputeMVP(pyramid.GetTransform(), camera),
                    pyramid.GetObjectType(), t);

            if (input.ShowCube())
                renderer.Render(*cube.GetPrimitive(),
                    ComputeMVP(cube.GetTransform(), camera),
                    cube.GetObjectType(), t);

            if (input.ShowOrtho())
                renderer.Render(*ortho.GetPrimitive(),
                    ComputeMVP(ortho.GetTransform(), camera),
                    ortho.GetObjectType(), t);

            // Models OBJ
            if (troll.IsVisible())
                renderer.Render(*troll.GetModel(),
                    ComputeMVP(troll.GetTransform(), camera), t);

            if (rock.IsVisible())
                renderer.Render(*rock.GetModel(),
                    ComputeMVP(rock.GetTransform(), camera), t);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}