#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdlib>
#include <chrono>

#include "Utils.h"
#include "Model.h"
#include "ModelGameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "Camera.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

const int OBJECT_TYPE_MODEL = 3;
const int OBJECT_TYPE_TINTED_SOLID = 4;

// Posiciones de los trolls 
const glm::vec3 TROLL_OFFSET(-1.5f, 0.0f, 0.0f);
const glm::vec3 TROLL_OFFSET2(-0.5f, 0.0f, -2.0f);
const glm::vec3 TROLL_OFFSET3(-0.5f, 0.0f, 2.0f);

const float FIRE_RADIUS = 0.8f;
const glm::vec3 ROCK_OFFSET(FIRE_RADIUS * cos(glm::radians(0.0f)), 0.0f, FIRE_RADIUS * sin(glm::radians(0.0f)));
const glm::vec3 ROCK_OFFSET2(FIRE_RADIUS * cos(glm::radians(120.0f)), 0.0f, FIRE_RADIUS * sin(glm::radians(120.0f)));
const glm::vec3 ROCK_OFFSET3(FIRE_RADIUS * cos(glm::radians(240.0f)), 0.0f, FIRE_RADIUS * sin(glm::radians(240.0f)));

const glm::vec3 DOG_OFFSET(0.0f, -0.5f, 0.0f);
const glm::vec3 CLOUD_OFFSET(5.0f, 2.0f, 10.0f);

const float CYCLE_DURATION = 20.0f; 
const float ORBIT_RADIUS = 5.0f;   

const float CAMERA_FOV = 70.0f;
const float CAMERA_ORBIT_RADIUS = 5.0f;
const float CAMERA_ORBIT_PITCH = 20.0f;
const float CAMERA_ORBIT_SPEED = 10.0f; 

const float DOLLY_START_DISTANCE = 10.0f;
const float DOLLY_MIN_DISTANCE = 2.0f;
const float DOLLY_SPEED = 2.0f;
const float DETAIL_VIEW_FOV = 25.0f;

const float FLASHLIGHT_INNER_CUTOFF_DEG = 12.5f;
const float FLASHLIGHT_OUTER_CUTOFF_DEG = 20.0f;
const float FLASHLIGHT_RANGE = 5.0f;

void RandomizeTransform(ModelGameObject& obj, float minX, float maxX, float minY, float maxY, float minZ, float maxZ, float minScale, float maxScale)
{
    auto randRange = [](float mn, float mx) 
    {
        return mn + static_cast<float>(rand()) / RAND_MAX * (mx - mn);
    };

    obj.GetTransform().SetPosition(glm::vec3(randRange(minX, maxX), randRange(minY, maxY), randRange(minZ, maxZ)));
    obj.GetTransform().SetScale(glm::vec3(randRange(minScale, maxScale)));
    obj.GetTransform().SetRotation(glm::vec3(randRange(0.0f, glm::two_pi<float>()), randRange(0.0f, glm::two_pi<float>()), randRange(0.0f, glm::two_pi<float>())));
}

glm::mat4 ComputeMVP(const Transform& transform, const Camera& camera)
{
    return camera.GetProjectionMatrix() * camera.GetViewMatrix() * transform.GetModelMatrix();
}

int main()
{
    srand(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "AR3 - Modelos y Camara", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindow);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    RenderManager renderer;
    renderer.Initialize(
        "VertexShader.glsl",
        "GeometryShader.glsl",
        "FragmentShader.glsl"
    );

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

    troll.GetTransform().SetPosition(TROLL_OFFSET);
    troll2.GetTransform().SetPosition(TROLL_OFFSET2);
    troll3.GetTransform().SetPosition(TROLL_OFFSET3);
    troll.GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    troll2.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    troll3.GetTransform().SetRotation(glm::vec3(0.0f, -110.0f, 0.0f));

    rock.GetTransform().SetPosition(ROCK_OFFSET);
    rock.GetTransform().SetScale(glm::vec3(0.4f));
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

    RandomizeTransform(troll, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1.2f);
    RandomizeTransform(rock, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 1.2f);
    RandomizeTransform(dog, -3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f, 0.3f, 0.5f);

    Camera camera(CAMERA_FOV, ASPECT_RATIO);
    glm::vec3 target = troll.GetTransform().GetPosition();
    camera.SetTarget(target);

    TimeManager time;

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Orbita de la cámara
    float yaw = 0.0f;
    float pitch = CAMERA_ORBIT_PITCH;
    float radius = CAMERA_ORBIT_RADIUS;

    // Modos de cámara
    bool  generalView = false;
    bool  detailView = false;
    bool  dollyZoom = false;
    float dollyDistance = DOLLY_START_DISTANCE;

    // Linterna
    bool flashlightOn = false;
    bool fKeyWasPressed = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        time.Update();

        const float t = time.GetTime();

        // Ciclo dia/noche
        float cycleAngle = (t / CYCLE_DURATION) * glm::two_pi<float>();
        glm::vec3 sunPos(ORBIT_RADIUS * cos(cycleAngle), ORBIT_RADIUS * sin(cycleAngle), 0.0f);
        glm::vec3 moonPos = -sunPos;
        glm::vec3 sunDir = glm::normalize(sunPos);
        glm::vec3 moonDir = glm::normalize(moonPos);

        float sunIntensity = glm::smoothstep(0.0f, 0.3f, sunDir.y);
        float moonIntensity = glm::smoothstep(0.0f, 0.3f, moonDir.y) * 0.4f;
        float dayBlend = (sunDir.y + 1.0f) * 0.5f;

        glm::vec3 nightAmbient(0.0f, 0.05f, 0.25f);
        glm::vec3 dayAmbient(0.4f, 0.35f, 0.05f);
        glm::vec3 ambientColor = glm::mix(nightAmbient, dayAmbient, dayBlend);
        float ambientStr = glm::mix(0.15f, 0.45f, dayBlend);

        renderer.SetDayNightUniforms(sunDir, sunIntensity, moonDir, moonIntensity, ambientColor, ambientStr);

        // Toggle de linterna
        bool fKeyNow = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
        if (fKeyNow && !fKeyWasPressed) flashlightOn = !flashlightOn;
        fKeyWasPressed = fKeyNow;

        glm::vec3 cameraPos;

        if (!generalView && !detailView && !dollyZoom)
        {
            yaw += CAMERA_ORBIT_SPEED * time.GetDeltaTime();

            cameraPos.x = target.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            cameraPos.y = target.y + radius * sin(glm::radians(pitch));
            cameraPos.z = target.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
            camera.SetTarget(target);
        }
        else
        {
            cameraPos = target + glm::vec3(0.0f, 2.0f, 10.0f);
            camera.SetTarget(target);
        }

        if (dollyZoom)
        {
            dollyDistance -= time.GetDeltaTime() * DOLLY_SPEED;
            if (dollyDistance < DOLLY_MIN_DISTANCE) dollyDistance = DOLLY_START_DISTANCE;
            cameraPos = target + glm::vec3(0.0f, 2.0f, dollyDistance);
            camera.SetFOV(glm::degrees(2.0f * atan(2.0f / dollyDistance)));
        }

        if (detailView)
        {
            glm::vec3 headTarget = target + glm::vec3(0.0f, 1.5f, 0.0f);
            cameraPos = headTarget + glm::vec3(0.0f, 0.2f, 1.0f);
            camera.SetTarget(headTarget);
            camera.SetFOV(DETAIL_VIEW_FOV);
        }

        camera.GetTransform().SetPosition(cameraPos);

        glm::vec3 flashDir = glm::normalize(target - cameraPos);
        renderer.SetFlashlightUniforms(flashlightOn, cameraPos, flashDir, glm::cos(glm::radians(FLASHLIGHT_INNER_CUTOFF_DEG)), glm::cos(glm::radians(FLASHLIGHT_OUTER_CUTOFF_DEG)), FLASHLIGHT_RANGE);

        renderer.Clear();

        glm::vec3 camPos = camera.GetTransform().GetPosition();
        glm::vec3 camForward = glm::normalize(camera.GetTarget() - camPos);

        auto renderModel = [&](ModelGameObject& obj,
            int type,
            glm::vec4 tintColor = glm::vec4(1.0f),
            
            float tintStrength = 0.0f)
            {
                if (!obj.IsVisible()) return;

                renderer.SetLight(obj.GetTransform().GetModelMatrix(),camPos, camForward, flashlightOn);
                renderer.Render(*obj.GetModel(), ComputeMVP(obj.GetTransform(), camera), obj.GetTransform().GetModelMatrix(), t, type, tintColor, tintStrength);
            };

        renderModel(troll, OBJECT_TYPE_MODEL, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.0f);
        renderModel(troll2, OBJECT_TYPE_MODEL);
        renderModel(troll3, OBJECT_TYPE_MODEL, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 1.0f);
        renderModel(rock, OBJECT_TYPE_MODEL);
        renderModel(rock2, OBJECT_TYPE_MODEL);
        renderModel(rock3, OBJECT_TYPE_MODEL);
        renderModel(dog, OBJECT_TYPE_MODEL);
        renderModel(cloud, OBJECT_TYPE_TINTED_SOLID, glm::vec4(0.3f, 0.3f, 0.3f, 0.3f), 1.0f);

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
            radius = CAMERA_ORBIT_RADIUS;
            pitch = CAMERA_ORBIT_PITCH;
            camera.SetFOV(CAMERA_FOV);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
