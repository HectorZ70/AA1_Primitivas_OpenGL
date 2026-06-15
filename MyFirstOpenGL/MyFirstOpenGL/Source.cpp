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
const float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

// Primitivas
const glm::vec3 PYRAMID_OFFSET(0.6f, 0.0f, 0.0f);
const glm::vec3 CUBE_OFFSET(-0.6f, 0.0f, 0.0f);
const glm::vec3 ORTHO_OFFSET(0.0f, 0.0f, 0.0f);

// Trolls
const glm::vec3 TROLL_OFFSET(-1.5f, 0.0f, 0.0f);
const glm::vec3 TROLL_OFFSET2(-0.5f, 0.0f, -2.0f);
const glm::vec3 TROLL_OFFSET3(-0.5f, 0.0f, 2.0f);

// Rocas en círculo
const float FIRE_RADIUS = 0.8f;
const glm::vec3 ROCK_OFFSET(
    FIRE_RADIUS* cos(glm::radians(0.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(0.0f)));
const glm::vec3 ROCK_OFFSET2(
    FIRE_RADIUS* cos(glm::radians(120.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(120.0f)));
const glm::vec3 ROCK_OFFSET3(
    FIRE_RADIUS* cos(glm::radians(240.0f)), 0.0f,
    FIRE_RADIUS* sin(glm::radians(240.0f)));

const glm::vec3 DOG_OFFSET(0.0f, -0.5f, 0.0f);
const glm::vec3 CLOUD_OFFSET(5.0f, 2.0f, 10.0f);
const glm::vec3 CLOUD_OFFSET2(10.0f, 2.0f, 10.0f);

// Escena vacía
const glm::vec3 PRAC_CUBE_OFFSET(-1.2f, 0.0f, 0.0f);
const glm::vec3 PRAC_ORTHO_OFFSET(0.0f, 0.0f, 0.0f);
const glm::vec3 PRAC_PYRAMID_OFFSET(1.2f, 0.0f, 0.0f);
const float     BOUNCE_AMPLITUDE = 0.5f;
const glm::vec3 ORTHO_BASE_SCALE(1.0f, 0.5f, 0.3f);
const glm::vec3 ORTHO_CUBE_SCALE(0.5f, 0.5f, 0.5f);
const glm::vec3 PRAC_CAM_POS(0.0f, 0.0f, 4.0f);
const glm::vec3 PRAC_CAM_TARGET(0.0f, 0.0f, 0.0f);

const float moveSpeed = 0.5f;
const float CYCLE_DURATION = 20.0f;
const float ORBIT_RADIUS = 5.0f;
float       mouseSensitivity = 0.1f;
double      lastMouseX = WINDOW_WIDTH / 2.0;
double      lastMouseY = WINDOW_HEIGHT / 2.0;

void RandomizeTransform(ModelGameObject& obj,
    float minX, float maxX,
    float minY, float maxY,
    float minZ, float maxZ,
    float minScale, float maxScale)
{
    auto randRange = [](float mn, float mx) {
        return mn + static_cast<float>(rand()) / RAND_MAX * (mx - mn);
        };
    obj.GetTransform().SetPosition(glm::vec3(
        randRange(minX, maxX),
        randRange(minY, maxY),
        randRange(minZ, maxZ)));
    obj.GetTransform().SetScale(glm::vec3(randRange(minScale, maxScale)));
    obj.GetTransform().SetRotation(glm::vec3(
        randRange(0.0f, glm::two_pi<float>()),
        randRange(0.0f, glm::two_pi<float>()),
        randRange(0.0f, glm::two_pi<float>())));
}

glm::mat4 ComputeMVP(const Transform& t, const Camera& cam)
{
    return cam.GetProjectionMatrix() * cam.GetViewMatrix() * t.GetModelMatrix();
}

int main()
{
    srand(static_cast<unsigned int>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, "Practica OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindow);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    RenderManager renderer;
    renderer.Initialize(
        "MyFirstVertexShader.glsl",
        "MyFirstGeometryShader.glsl",
        "MyFirstFragmentShader.glsl"
    );

    Primitive pyramidMesh;
    pyramidMesh.SetVerticesAndVariables(
        { -0.5f,0.0f,-0.5f,  0.5f,0.0f,-0.5f,
           0.5f,0.0f, 0.5f, -0.5f,0.0f, 0.5f,
           0.0f,0.8f, 0.0f },
        { 0,1,2, 2,3,0,
          0,1,4, 1,2,4, 2,3,4, 3,0,4 });

    Primitive cubeMesh;
    cubeMesh.SetVerticesAndVariables(
        { -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
          -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
           0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f },
        { 0,1,2, 2,3,0,   5,4,7, 7,6,5,
          4,0,3, 3,7,4,   1,5,6, 6,2,1,
          3,2,6, 6,7,3,   4,5,1, 1,0,4 });

    Primitive orthoMesh;
    orthoMesh.SetVerticesAndVariables(
        { -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
          -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
           0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f },
        { 0,1,2, 2,3,0,   5,4,7, 7,6,5,
          4,0,3, 3,7,4,   1,5,6, 6,2,1,
          3,2,6, 6,7,3,   4,5,1, 1,0,4 });

    GameObject pyramid(&pyramidMesh, 0);
    GameObject cube(&cubeMesh, 1);
    GameObject ortho(&orthoMesh, 2);

    GameObject pracCube(&cubeMesh, 0);
    GameObject pracOrtho(&orthoMesh, 1);
    GameObject pracPyramid(&pyramidMesh, 2);

    Model trollModel = LoadOBJModel("Assets/Modelos/troll.obj");
    Model rockModel = LoadOBJModel("Assets/Modelos/rock.obj");
    Model dogModel = LoadOBJModel("Assets/Modelos/dog.obj");
    trollModel.SetTexture(LoadTexture("Assets/Texturas/troll.png"));
    rockModel.SetTexture(LoadTexture("Assets/Texturas/rock.png"));
    dogModel.SetTexture(LoadTexture("Assets/Texturas/dog.png"));

    ModelGameObject troll(&trollModel);
    ModelGameObject troll2(&trollModel);
    ModelGameObject troll3(&trollModel);
    ModelGameObject rock(&rockModel);
    ModelGameObject rock2(&rockModel);
    ModelGameObject rock3(&rockModel);
    ModelGameObject dog(&dogModel);
    ModelGameObject cloud(&rockModel);
    ModelGameObject largeCloud(&rockModel);

    troll.GetTransform().SetPosition(TROLL_OFFSET);
    troll2.GetTransform().SetPosition(TROLL_OFFSET2);
    troll3.GetTransform().SetPosition(TROLL_OFFSET3);
    troll.GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    troll2.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    troll3.GetTransform().SetRotation(glm::vec3(0.0f, -110.0f, 0.0f));

    rock.GetTransform().SetPosition(ROCK_OFFSET);
    rock.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
    rock.GetTransform().SetScale(glm::vec3(0.4f));
    largeCloud.GetTransform().SetWiden(5.0f);
    largeCloud.GetTransform().SetPosition(glm::vec3(10.f, 10.0f, 0.0f));

    rock2.GetTransform().SetPosition(ROCK_OFFSET2);
    rock2.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(120.0f), 0.0f));
    rock2.GetTransform().SetScale(glm::vec3(0.4f));
    rock3.GetTransform().SetPosition(ROCK_OFFSET3);
    rock3.GetTransform().SetRotation(glm::vec3(0.0f, glm::radians(240.0f), 0.0f));
    rock3.GetTransform().SetScale(glm::vec3(0.4f));

    dog.GetTransform().SetPosition(DOG_OFFSET);
    cloud.GetTransform().SetPosition(CLOUD_OFFSET);
    cloud.GetTransform().SetScale(glm::vec3(1.4f));
    cloud.GetTransform().SetRotation(glm::vec3(10.0f, 5.0f, 5.0f));

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
    float pitch = 20.0f;

    const float defaultRadius = 5.0f;
    const float defaultPitch = 20.0f;
    const float defaultFOV = 70.0f;

    float radius = defaultRadius;
    float dollyDistance = 10.0f;
    float dollySpeed = 2.0f;

    bool generalView = false;
    bool detailView = false;
    bool dollyZoom = false;

    bool flashOn = true;   
    bool flashlightOn = false;  
    bool fWasPressed = false;
    bool fKeyWasPressed = false;

    bool wasGameScene = true;

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

        float cycleAngle = (t / CYCLE_DURATION) * glm::two_pi<float>();
        glm::vec3 sunPos(ORBIT_RADIUS * cos(cycleAngle),
            ORBIT_RADIUS * sin(cycleAngle), 0.0f);
        glm::vec3 moonPos = -sunPos;
        glm::vec3 sunDir = glm::normalize(sunPos);
        glm::vec3 moonDir = glm::normalize(moonPos);

        float sunIntensity = glm::smoothstep(0.0f, 0.3f, sunDir.y);
        float moonIntensity = glm::smoothstep(0.0f, 0.3f, moonDir.y) * 0.4f;
        float dayBlend = (sunDir.y + 1.0f) * 0.5f;

        glm::vec3 nightAmbient(0.0f, 0.05f, 0.25f);
        glm::vec3 dayAmbient(0.4f, 0.35f, 0.05f);
        glm::vec3 ambientColor = glm::mix(nightAmbient, dayAmbient, dayBlend);
        float     ambientStr = glm::mix(0.15f, 0.45f, dayBlend);

        renderer.SetDayNightUniforms(
            sunDir, sunIntensity,
            moonDir, moonIntensity,
            ambientColor, ambientStr);

        bool fKeyNow = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
        if (fKeyNow && !fKeyWasPressed) flashlightOn = !flashlightOn;
        fKeyWasPressed = fKeyNow;

        bool fPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
        if (fPressed && !fWasPressed) flashOn = !flashOn;
        fWasPressed = fPressed;

        glm::vec3 cameraPos;

        if (!generalView && !detailView && !dollyZoom)
        {
            glm::vec3 forward(
                cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                0.0f,
                cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                target += forward * moveSpeed * time.GetDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                target -= forward * moveSpeed * time.GetDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                target -= right * moveSpeed * time.GetDeltaTime();
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                target += right * moveSpeed * time.GetDeltaTime();

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            yaw -= (float)(mouseX - lastMouseX) * mouseSensitivity;
            pitch += (float)(mouseY - lastMouseY) * mouseSensitivity;
            pitch = glm::clamp(pitch, -89.0f, 89.0f);
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            cameraPos.x = target.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            cameraPos.y = target.y + radius * sin(glm::radians(pitch));
            cameraPos.z = target.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            camera.SetTarget(target);
        }
        else
        {
            cameraPos = target + glm::vec3(0.0f, 2.0f, 10.0f);
        }

        if (dollyZoom)
        {
            dollyDistance -= time.GetDeltaTime() * dollySpeed;
            if (dollyDistance < 2.0f) dollyDistance = 10.0f;
            cameraPos = target + glm::vec3(0.0f, 2.0f, dollyDistance);
            camera.SetFOV(glm::degrees(2.0f * atan(2.0f / dollyDistance)));
        }
        if (detailView)
        {
            glm::vec3 headTarget = target + glm::vec3(0.0f, 1.5f, 0.0f);
            cameraPos = headTarget + glm::vec3(0.0f, 0.2f, 1.0f);
            camera.SetTarget(headTarget);
            camera.SetFOV(25.0f);
        }

        camera.GetTransform().SetPosition(cameraPos);

        glm::vec3 flashDir = glm::normalize(target - cameraPos);
        renderer.SetFlashlightUniforms(
            flashlightOn,
            cameraPos,
            flashDir,
            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(20.0f)),
            5.0f);

        renderer.Clear();

        if (sceneManager.IsGameScene())
        {
            wasGameScene = true;

            glm::vec3 camPos = camera.GetTransform().GetPosition();
            glm::vec3 camForward = glm::normalize(camera.GetTarget() - camPos);

            // Lambda: SetLight (HEAD) + Render con tint
            auto renderModel = [&](ModelGameObject& obj,
                int       type,
                glm::vec4 tintColor = glm::vec4(1.0f),
                float     tintStrength = 0.0f)
                {
                    if (!obj.IsVisible()) return;
                    renderer.SetLight(obj.GetTransform().GetModelMatrix(),
                        camPos, camForward, flashOn);
                    renderer.Render(*obj.GetModel(),
                        ComputeMVP(obj.GetTransform(), camera),
                        obj.GetTransform().GetModelMatrix(),
                        t, type, tintColor, tintStrength);
                };

            // Primitivas animadas
            pyramid.GetTransform().SetPosition(
                PYRAMID_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
            pyramid.GetTransform().SetRotation(glm::vec3(t, t, 0.0f));
            pyramid.GetTransform().SetScale(glm::vec3(0.5f));

            cube.GetTransform().SetPosition(
                CUBE_OFFSET + glm::vec3(0.0f, sin(t) * 0.75f, 0.0f));
            cube.GetTransform().SetRotation(glm::vec3(0.0f, t * 2.0f, 0.0f));
            cube.GetTransform().SetScale(glm::vec3(0.5f));

            float blend = (sin(t) + 1.0f) * 0.5f;
            ortho.GetTransform().SetPosition(ORTHO_OFFSET);
            ortho.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, t * 2.0f));
            ortho.GetTransform().SetScale(glm::mix(
                glm::vec3(1.0f, 0.5f, 0.3f), glm::vec3(0.5f), blend));

            // Modelos OBJ
            renderModel(troll, 3, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f);
            renderModel(troll2, 3);
            renderModel(troll3, 3, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
            renderModel(rock, 3);
            renderModel(rock2, 3);
            renderModel(rock3, 3);
            renderModel(dog, 3);
            renderModel(largeCloud, 3);
            renderModel(cloud, 3);

            if (cloud.IsVisible())
            {
                renderer.SetLight(cloud.GetTransform().GetModelMatrix(),
                    camPos, camForward, flashOn);
                renderer.Render(*cloud.GetModel(),
                    ComputeMVP(cloud.GetTransform(), camera),
                    cloud.GetTransform().GetModelMatrix(),
                    t, 4, glm::vec4(0.3f, 0.3f, 0.3f, 0.3f), 1.0f);
            }

            // Teclas de modo de cámara
            if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            {
                generalView = true;  detailView = false; dollyZoom = false;
            }
            if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            {
                detailView = true;  generalView = false; dollyZoom = false;
            }
            if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            {
                dollyZoom = true;  generalView = false; detailView = false;
            }
            if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
            {
                generalView = false; detailView = false; dollyZoom = false;
                radius = defaultRadius;
                pitch = defaultPitch;
                camera.SetFOV(defaultFOV);
            }
        }
        else if (sceneManager.IsEmptyScene())
        {
            if (wasGameScene)
            {
                generalView = false;
                detailView = false;
                dollyZoom = false;
                camera.SetFOV(defaultFOV);
                input.Reset();
                wasGameScene = false;
            }

            camera.SetFOV(defaultFOV);
            camera.SetTarget(PRAC_CAM_TARGET);
            camera.GetTransform().SetPosition(PRAC_CAM_POS);

            // Cubo
            pracCube.GetTransform().SetPosition(
                PRAC_CUBE_OFFSET + glm::vec3(0.0f, sin(t) * BOUNCE_AMPLITUDE, 0.0f));
            pracCube.GetTransform().SetRotation(glm::vec3(0.0f, t, 0.0f));
            pracCube.GetTransform().SetScale(glm::vec3(0.5f));

            // Ortho
            float blend = (sin(t) + 1.0f) * 0.5f;
            pracOrtho.GetTransform().SetPosition(PRAC_ORTHO_OFFSET);
            pracOrtho.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, t));
            pracOrtho.GetTransform().SetScale(
                glm::mix(ORTHO_BASE_SCALE, ORTHO_CUBE_SCALE, blend));

            // Pirámide
            pracPyramid.GetTransform().SetPosition(
                PRAC_PYRAMID_OFFSET + glm::vec3(0.0f, sin(t) * BOUNCE_AMPLITUDE, 0.0f));
            pracPyramid.GetTransform().SetRotation(glm::vec3(t, t, 0.0f));
            pracPyramid.GetTransform().SetScale(glm::vec3(0.5f));

            if (input.ShowCube())
                renderer.Render(*pracCube.GetPrimitive(),
                    ComputeMVP(pracCube.GetTransform(), camera),
                    pracCube.GetTransform().GetModelMatrix(),
                    pracCube.GetObjectType(), t);
            if (input.ShowOrtho())
                renderer.Render(*pracOrtho.GetPrimitive(),
                    ComputeMVP(pracOrtho.GetTransform(), camera),
                    pracOrtho.GetTransform().GetModelMatrix(),
                    pracOrtho.GetObjectType(), t);
            if (input.ShowPyramid())
                renderer.Render(*pracPyramid.GetPrimitive(),
                    ComputeMVP(pracPyramid.GetTransform(), camera),
                    pracPyramid.GetTransform().GetModelMatrix(),
                    pracPyramid.GetObjectType(), t);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}