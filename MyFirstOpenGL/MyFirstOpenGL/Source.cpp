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
const glm::vec3 TROLL_OFFSET(-1.5f, 0.0f, 0.0f);
const glm::vec3 TROLL_OFFSET2(-0.5f, 0.0f, -2.0f);
const glm::vec3 TROLL_OFFSET3(-0.5f, 0.0f, 2.0f);
const float FIRE_RADIUS = 0.8f;

// Posiciones en círculo (ángulos: 0°, 120°, 240°)
const glm::vec3 ROCK_OFFSET(
    FIRE_RADIUS* cos(glm::radians(0.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(0.0f)));

const glm::vec3 ROCK_OFFSET2(
    FIRE_RADIUS* cos(glm::radians(120.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(120.0f)));

const glm::vec3 ROCK_OFFSET3(
    FIRE_RADIUS* cos(glm::radians(240.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(240.0f)));


const glm::vec3 CLOUD_OFFSET(5.0f, 2.0f, 10.f);

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

    Model trollModel = LoadOBJModel("Assets/Modelos/troll.obj");
    Model trollModel2 = LoadOBJModel("Assets/Modelos/troll.obj");
    Model rockModel = LoadOBJModel("Assets/Modelos/rock.obj");
    trollModel.SetTexture(LoadTexture("Assets/Texturas/troll.png"));
    rockModel.SetTexture(LoadTexture("Assets/Texturas/rock.png"));

    ModelGameObject troll(&trollModel);
    ModelGameObject troll2(&trollModel);
    ModelGameObject troll3(&trollModel);
    ModelGameObject rock(&rockModel);
    ModelGameObject cloud(&rockModel);
    troll.GetTransform().SetPosition(TROLL_OFFSET);
    troll2.GetTransform().SetPosition(TROLL_OFFSET2);
    troll3.GetTransform().SetPosition(TROLL_OFFSET3);

    troll.GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    troll2.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    troll3.GetTransform().SetRotation(glm::vec3(0.0f, -110.0f, 0.0f));

    rock.GetTransform().SetPosition(ROCK_OFFSET);
    cloud.GetTransform().SetPosition(CLOUD_OFFSET);

    ModelGameObject rock2(&rockModel);
    ModelGameObject rock3(&rockModel);
    rock2.GetTransform().SetPosition(ROCK_OFFSET2);
    rock3.GetTransform().SetPosition(ROCK_OFFSET3);

    rock.GetTransform().SetPosition(ROCK_OFFSET);
    rock.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));

    rock2.GetTransform().SetPosition(ROCK_OFFSET2);
    rock2.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(120.0f), 0.0f));

    rock3.GetTransform().SetPosition(ROCK_OFFSET3);
    rock3.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(240.0f), 0.0f));

    rock.GetTransform().SetScale(glm::vec3(0.4f));
    rock2.GetTransform().SetScale(glm::vec3(0.4f));
    rock3.GetTransform().SetScale(glm::vec3(0.4f));

    cloud.GetTransform().SetScale(glm::vec3(1.4f));
    cloud.GetTransform().SetRotation(glm::vec3(120.0f, 0.f, 0.0f));

    Camera camera(70.0f, ASPECT_RATIO);
    camera.GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::vec3 target = troll.GetTransform().GetPosition();

    camera.SetTarget(target);

    InputManager input;
    SceneManager sceneManager;
    TimeManager  time;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float yaw = 0.0f;

    float defaultRadius = 5.0f;
    float defaultPitch = 20.0f;
    float defaultFOV = 70.0f;

    float radius = defaultRadius;
    float pitch = defaultPitch;

    float dollyDistance = 10.0f;
    float dollySpeed = 2.0f;

    bool generalView = false;
    bool detailView = false;
    bool dollyZoom = false;

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

        //Rotación de la camara
        glm::vec3 cameraPos;

        if (!generalView &&
            !detailView &&
            !dollyZoom)
        {
            yaw += time.GetDeltaTime() * 50.0f;

            cameraPos.x =
                target.x +
                radius *
                cos(glm::radians(pitch)) *
                cos(glm::radians(yaw));

            cameraPos.y =
                target.y +
                radius *
                sin(glm::radians(pitch));

            cameraPos.z =
                target.z +
                radius *
                cos(glm::radians(pitch)) *
                sin(glm::radians(yaw));

            camera.SetTarget(target);
        }
        else
        {
            // Plano general frontal
            cameraPos =
                target +
                glm::vec3(0.0f, 2.0f, 10.0f);
        }

        if (dollyZoom)
        {
            dollyDistance -= time.GetDeltaTime() * dollySpeed;

            if (dollyDistance < 2.0f)
                dollyDistance = 10.0f;

            cameraPos =
                target +
                glm::vec3(0.0f, 2.0f, dollyDistance);

            float dynamicFOV =
                glm::degrees(
                    2.0f * atan(2.0f / dollyDistance)
                );

            camera.SetFOV(dynamicFOV);
        }
        if (detailView)
        {
            glm::vec3 headTarget =
                target + glm::vec3(0.0f, 1.5f, 0.0f);

            cameraPos =
                headTarget +
                glm::vec3(0.0f, 0.2f, 1.0f);

            camera.SetTarget(headTarget);

            camera.SetFOV(25.0f);
        }

        camera.GetTransform().SetPosition(cameraPos);
        renderer.Clear();

        if (sceneManager.IsGameScene())
        {
            // Models OBJ
            if (troll.IsVisible())
                renderer.Render(*troll.GetModel(),
                    ComputeMVP(troll.GetTransform(), camera), t, 3,
                    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f);

            
            if (troll2.IsVisible())
                renderer.Render(*troll2.GetModel(),
                    ComputeMVP(troll2.GetTransform(), camera), t, 3);

            if (troll3.IsVisible())
                renderer.Render(*troll3.GetModel(),
                    ComputeMVP(troll3.GetTransform(), camera), t, 3,
                glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);

            if (rock.IsVisible())
                renderer.Render(*rock.GetModel(),
                    ComputeMVP(rock.GetTransform(), camera), t, 3);

            if (rock2.IsVisible())
                renderer.Render(*rock2.GetModel(),
                    ComputeMVP(rock2.GetTransform(), camera), t, 3);

            if (rock3.IsVisible())
                renderer.Render(*rock3.GetModel(),
                    ComputeMVP(rock3.GetTransform(), camera), t, 3);
           
            if (cloud.IsVisible())
                renderer.Render(*cloud.GetModel(),
                    ComputeMVP(cloud.GetTransform(), camera), t,
                    4, glm::vec4(0.3f, 0.3f, 0.3f, .3f), 1.0f);

            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            {
                generalView = true;

                detailView = false;
                dollyZoom = false;
            }

            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            {
                detailView = true;

                generalView = false;
                dollyZoom = false;
            }

            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            {
                dollyZoom = true;

                generalView = false;
                detailView = false;
            }
             
            if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
            {
                generalView = false;
                detailView = false;
                dollyZoom = false;

                radius = defaultRadius;
                pitch = defaultPitch;

                camera.SetFOV(defaultFOV);
            }

           
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}