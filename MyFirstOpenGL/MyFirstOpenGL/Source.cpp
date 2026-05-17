#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "Utils.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Model.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stb_image.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const float SPEED_UP_FACTOR = 1.1f;
const float SPEED_DOWN_FACTOR = 0.9f;

const glm::vec2 PYRAMID_OFFSET(0.6f, 0.0f);
const glm::vec2 CUBE_OFFSET(-0.6f, 0.0f);
const glm::vec2 ORTHO_OFFSET(0.0f, 0.0f);

std::vector<Model> models;

Model LoadOBJModel(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    std::stringstream ss;
    std::string prefix;
    glm::vec3 tmpVec3;
    glm::vec2 tmpVec2;

    std::vector<float> vertexs;
    std::vector<float> vertexNormal;
    std::vector<float> textureCoordinates;

    std::vector<float> tmpVertexs;
    std::vector<float> tmpNormals;
    std::vector<float> tmpTextureCoordinates;

    while (std::getline(file, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v")
        {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

            tmpVertexs.push_back(tmpVec3.x);
            tmpVertexs.push_back(tmpVec3.y);
            tmpVertexs.push_back(tmpVec3.z);
        }
        else if (prefix == "vt")
        {
            //Las UVs son siempre imagenes 2D asi que uso el tmpvec2 para almacenarlas
            ss >> tmpVec2.x >> tmpVec2.y;

            //Almaceno en mi vector temporal las UVs
            tmpTextureCoordinates.push_back(tmpVec2.x);
            tmpTextureCoordinates.push_back(tmpVec2.y);
        }
        else if (prefix == "vn")
        {
            //Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

            //Almaceno en mi vector temporal de normales las normales
            tmpNormals.push_back(tmpVec3.x);
            tmpNormals.push_back(tmpVec3.y);
            tmpNormals.push_back(tmpVec3.z);

        }
        else if (prefix == "f")
        {
            int vertexData;
            short counter = 0;

            //Obtengo todos los valores hasta un espacio
            while (ss >> vertexData) {

                //En orden cada numero sigue el patron de vertice/uv/normal
                switch (counter) {
                case 0:
                    //Si es un vertice lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec3, salto 1 / y aumento el contador en 1
                    vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
                    vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
                    ss.ignore(1, '/');
                    counter++;
                    break;
                case 1:
                    //Si es un uv lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec2, salto 1 / y aumento el contador en 1
                    textureCoordinates.push_back(tmpTextureCoordinates[(vertexData - 1) * 2]);
                    textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
                    ss.ignore(1, '/');
                    counter++;
                    break;
                case 2:
                    //Si es una normal la almaceno - 1 por el offset y almaceno tres seguidos al ser un vec3, salto 1 / y reinicio
                    vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
                    vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
                    counter = 0;
                    break;
                }
            }
        }
    }
    return Model(vertexs, textureCoordinates, vertexNormal);
}

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

    models.push_back(LoadOBJModel("Assets/Modelos/troll.obj"));

    glActiveTexture(GL_TEXTURE0);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* textureInfo = stbi_load("Assets/Texturas/troll.png", &width, &height, &nrChannels, 0);
    if (!textureInfo)
        std::cerr << "Error cargando textura: " << stbi_failure_reason() << std::endl;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureInfo);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(textureInfo);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    RenderManager renderer;   // también tienes esto duplicado, borra una copia
    renderer.Initialize("MyFirstVertexShader.glsl", "MyFirstFragmentShader.glsl");

    // DESPUÉS de Initialize ya tienes el shaderProgram disponible:
    glUseProgram(renderer.GetProgram());
    glUniform1i(glGetUniformLocation(renderer.GetProgram(), "textureSampler"), 0);

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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
            models[0].Render(renderer.GetProgram(), time.GetTime(), glm::vec3(-0.6f, 0.0f, 0.0f));
            models[0].Render(renderer.GetProgram(), time.GetTime(), glm::vec3( 0.6f, 0.0f, 0.0f));
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