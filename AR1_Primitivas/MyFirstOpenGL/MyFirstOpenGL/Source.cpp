#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <cmath>

#include "Utils.h"
#include "Primitive.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Camera.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

const int OBJECT_TYPE_CUBE = 0;
const int OBJECT_TYPE_ORTHO = 1;
const int OBJECT_TYPE_PYRAMID = 2;

const glm::vec3 CUBE_OFFSET(-1.2f, 0.0f, 0.0f);
const glm::vec3 ORTHO_OFFSET(0.0f, 0.0f, 0.0f);
const glm::vec3 PYRAMID_OFFSET(1.2f, 0.0f, 0.0f);

const float BOUNCE_AMPLITUDE = 0.5f;
const glm::vec3 ORTHO_BASE_SCALE(1.0f, 0.5f, 0.3f);
const glm::vec3 ORTHO_CUBE_SCALE(0.5f, 0.5f, 0.5f);
const glm::vec3 PRIMITIVE_SCALE(0.5f);

const glm::vec3 CAMERA_POSITION(0.0f, 0.0f, 4.0f);
const glm::vec3 CAMERA_TARGET(0.0f, 0.0f, 0.0f);
const float CAMERA_FOV = 70.0f;

glm::mat4 ComputeMVP(const Transform& transform, const Camera& camera)
{
    return camera.GetProjectionMatrix() * camera.GetViewMatrix() * transform.GetModelMatrix();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "AR1 - Primitivas OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindow);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);

    RenderManager renderer;
    renderer.Initialize(
        "VertexShader.glsl",
        "GeometryShader.glsl",
        "FragmentShader.glsl"
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

    GameObject cube(&cubeMesh, OBJECT_TYPE_CUBE);
    GameObject ortho(&orthoMesh, OBJECT_TYPE_ORTHO);
    GameObject pyramid(&pyramidMesh, OBJECT_TYPE_PYRAMID);

    Camera camera(CAMERA_FOV, ASPECT_RATIO);
    camera.GetTransform().SetPosition(CAMERA_POSITION);
    camera.SetTarget(CAMERA_TARGET);

    InputManager input;
    TimeManager time;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        input.Update(window, time);
        time.Update();

        const float t = time.GetTime();

        // Cubo
        cube.GetTransform().SetPosition(CUBE_OFFSET + glm::vec3(0.0f, sin(t) * BOUNCE_AMPLITUDE, 0.0f));
        cube.GetTransform().SetRotation(glm::vec3(0.0f, t, 0.0f));
        cube.GetTransform().SetScale(PRIMITIVE_SCALE);

        // Ortoedro
        float blend = (sin(t) + 1.0f) * 0.5f;
        ortho.GetTransform().SetPosition(ORTHO_OFFSET);
        ortho.GetTransform().SetRotation(glm::vec3(0.0f, 0.0f, t));
        ortho.GetTransform().SetScale(glm::mix(ORTHO_BASE_SCALE, ORTHO_CUBE_SCALE, blend));

        // Piramide
        pyramid.GetTransform().SetPosition(PYRAMID_OFFSET + glm::vec3(0.0f, sin(t) * BOUNCE_AMPLITUDE, 0.0f));
        pyramid.GetTransform().SetRotation(glm::vec3(t, t, 0.0f));
        pyramid.GetTransform().SetScale(PRIMITIVE_SCALE);

        renderer.Clear();

        if (input.ShowCube())
        {
            renderer.Render(*cube.GetPrimitive(), ComputeMVP(cube.GetTransform(), camera), cube.GetObjectType(), t);
        }
            
        if (input.ShowOrtho())
        {
            renderer.Render(*ortho.GetPrimitive(), ComputeMVP(ortho.GetTransform(), camera), ortho.GetObjectType(), t);
        }

        if (input.ShowPyramid())
        {
            renderer.Render(*pyramid.GetPrimitive(), ComputeMVP(pyramid.GetTransform(), camera), pyramid.GetObjectType(), t);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
