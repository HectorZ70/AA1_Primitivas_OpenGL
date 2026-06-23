#include "Utils.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <sstream>
#include <fstream>
#include <glm.hpp>
#include <stb_image.h>
#include "Model.h"

Model LoadOBJModel(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "No se ha podido abrir: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string line, prefix;
    std::stringstream ss;
    glm::vec3 tmpVec3;
    glm::vec2 tmpVec2;

    std::vector<float> tmpVertices, tmpNormals, tmpUVs;
    std::vector<float> vertices, normals, uvs;

    while (std::getline(file, line))
    {
        ss.clear(); ss.str(line);
        ss >> prefix;

        if (prefix == "v")
        {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpVertices.insert(tmpVertices.end(),
                { tmpVec3.x, tmpVec3.y, tmpVec3.z });
        }
        else if (prefix == "vt")
        {
            ss >> tmpVec2.x >> tmpVec2.y;
            tmpUVs.insert(tmpUVs.end(), { tmpVec2.x, tmpVec2.y });
        }
        else if (prefix == "vn")
        {
            ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;
            tmpNormals.insert(tmpNormals.end(),
                { tmpVec3.x, tmpVec3.y, tmpVec3.z });
        }
        else if (prefix == "f")
        {
            int idx; short counter = 0;
            while (ss >> idx)
            {
                switch (counter)
                {
                case 0:
                    vertices.push_back(tmpVertices[(idx - 1) * 3]);
                    vertices.push_back(tmpVertices[(idx - 1) * 3 + 1]);
                    vertices.push_back(tmpVertices[(idx - 1) * 3 + 2]);
                    ss.ignore(1, '/'); counter++; 
                    break;
                case 1:
                    uvs.push_back(tmpUVs[(idx - 1) * 2]);
                    uvs.push_back(tmpUVs[(idx - 1) * 2 + 1]);
                    ss.ignore(1, '/'); counter++; 
                    break;
                case 2:
                    normals.push_back(tmpNormals[(idx - 1) * 3]);
                    normals.push_back(tmpNormals[(idx - 1) * 3 + 1]);
                    normals.push_back(tmpNormals[(idx - 1) * 3 + 2]);
                    counter = 0; 
                    break;
                }
            }
        }
    }
    return Model(vertices, uvs, normals);
}

GLuint LoadTexture(const std::string& filePath)
{
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &w, &h, &ch, 0);

    if (!data)
    {
        std::cerr << "Error cargando textura: " << filePath << std::endl;
        return 0;
    }

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return id;
}

void ResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

std::string LoadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::string content, line;

    if (!file.is_open()) 
    {
        std::cerr << "Error abriendo archivo: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(file, line))
    {
        content += line + "\n";
    }

    return content;
}

// Vertex Shader
GLuint LoadVertexShader(const std::string& path)
{
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    std::string code = LoadFile(path);
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Vertex shader error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

// GeometryShader
GLuint LoadGeometryShader(const std::string& path)
{
    GLuint shader = glCreateShader(GL_GEOMETRY_SHADER);
    std::string code = LoadFile(path);
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Geometry shader error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

// Fragment Shader
GLuint LoadFragmentShader(const std::string& path)
{
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string code = LoadFile(path);
    const char* src = code.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) 
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetShaderInfoLog(shader, len, nullptr, log.data());
        std::cerr << "Fragment shader error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}

GLuint CreateProgram(const ShaderProgram& shaders)
{
    GLuint program = glCreateProgram();

    if (shaders.vertexShader)
        glAttachShader(program, shaders.vertexShader);
    
    if (shaders.geometryShader)
        glAttachShader(program, shaders.geometryShader);

    if (shaders.fragmentShader)
        glAttachShader(program, shaders.fragmentShader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) 
    {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(len);
        glGetProgramInfoLog(program, len, nullptr, log.data());
        std::cerr << "Link error: " << log.data() << std::endl;
        exit(EXIT_FAILURE);
    }

    return program;
}