#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdlib>

#include "Utils.h"
#include "Primitive.h"
#include "GameObject.h"
#include "ModelGameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Camera.h"
#include <chrono>

const int   WINDOW_WIDTH = 640;
const int   WINDOW_HEIGHT = 480;
const float ASPECT_RATIO =
static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

const glm::vec3 PYRAMID_OFFSET(0.6f, 0.0f, 0.0f);
const glm::vec3 CUBE_OFFSET(-0.6f, 0.0f, 0.0f);
const glm::vec3 ORTHO_OFFSET(0.0f, 0.0f, 0.0f);
const glm::vec3 TROLL_OFFSET(-0.6f, 0.0f, 0.0f);
const glm::vec3 ROCK_OFFSET(1.6f, 0.0f, 0.0f);
const glm::vec3 DOG_OFFSET(0.0f, -.5f, 0.f);

const float moveSpeed = 0.5f;

void RandomizeTransform(ModelGameObject& obj,
    float minX, float maxX,
    float minY, float maxY,
    float minZ, float maxZ,
    float minScale, float maxScale)
{
    auto randRange = [](float min, float max) {
        return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
        };

    obj.GetTransform().SetPosition(glm::vec3(
        randRange(minX, maxX),
        randRange(minY, maxY),
        randRange(minZ, maxZ)
    ));
    obj.GetTransform().SetScale(glm::vec3(randRange(minScale, maxScale)));
    obj.GetTransform().SetRotation(glm::vec3(
        randRange(0.0f, glm::two_pi<float>()),  // X
        randRange(0.0f, glm::two_pi<float>()),  // Y
        randRange(0.0f, glm::two_pi<float>())   // Z
    ));
}

glm::mat4 ComputeMVP(const Transform& t, const Camera& cam)
{
    return cam.GetProjectionMatrix()
        * cam.GetViewMatrix()
        * t.GetModelMatrix();
}

int main()
{
    srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
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
    Model rockModel = LoadOBJModel("Assets/Modelos/rock.obj");
    Model dogModel = LoadOBJModel("Assets/Modelos/dog.obj");
    trollModel.SetTexture(LoadTexture("Assets/Texturas/troll.png"));
    rockModel.SetTexture(LoadTexture("Assets/Texturas/rock.png"));
    dogModel.SetTexture(LoadTexture("Assets/Texturas/dog.png"));

    ModelGameObject troll(&trollModel);
    ModelGameObject rock(&rockModel);
    ModelGameObject dog(&dogModel);
    troll.GetTransform().SetPosition(TROLL_OFFSET);
    rock.GetTransform().SetPosition(ROCK_OFFSET);
    dog.GetTransform().SetPosition(DOG_OFFSET);

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

    auto randRange = [](float min, float max) {
        return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
        };

    RandomizeTransform(troll, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1.2f);
    RandomizeTransform(rock, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1.2f);
    RandomizeTransform(dog, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 0.5f);

    target = troll.GetTransform().GetPosition();
    camera.SetTarget(target);


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
            //yaw += time.GetDeltaTime() * 50.0f;

            glm::vec3 forward(
                cos(glm::radians(pitch))* cos(glm::radians(yaw)),
                0.0f,
                cos(glm::radians(pitch))* sin(glm::radians(yaw))
            );
            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                target += forward * moveSpeed * time.GetDeltaTime();

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                target -= forward * moveSpeed * time.GetDeltaTime();

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                target -= right * moveSpeed * time.GetDeltaTime();

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                target += right * moveSpeed * time.GetDeltaTime();

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
                    ComputeMVP(troll.GetTransform(), camera), t);

            if (rock.IsVisible())
                renderer.Render(*rock.GetModel(),
                    ComputeMVP(rock.GetTransform(), camera), t);

            if (dog.IsVisible())
                renderer.Render(*dog.GetModel(),
                    ComputeMVP(dog.GetTransform(), camera), t);

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