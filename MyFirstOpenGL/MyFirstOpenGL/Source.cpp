#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Pyramid.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct ShaderProgram 
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) 
{
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
}

std::string Load_File(const std::string& filePath) 
{
	std::ifstream file(filePath);
	std::string fileContent;
	std::string line;

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	file.close();
	return fileContent;
}

GLuint LoadVertexShader(const std::string& filePath) {

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success) {
		return vertexShader;
	}
	else {
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		std::cerr << "Error al cargar el vertex shader: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint LoadFragmentShader(const std::string& filePath) {
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string code = Load_File(filePath);
	const char* source = code.c_str();

	glShaderSource(fragShader, 1, &source, nullptr);
	glCompileShader(fragShader);

	GLint success;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint logLength;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragShader, logLength, nullptr, errorLog.data());
		std::cerr << "Error al cargar el fragment shader: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return fragShader;
}

// Linkea shaders en un programa de OpenGL
GLuint CreateProgram(const ShaderProgram& shaders) {

	GLuint program = glCreateProgram();

	// Adjuntar shaders si existen
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}
	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	// Linkar programa
	glLinkProgram(program);

	// Verificar errores
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (success) {
		// Desadjuntar shaders después de linkear
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}
		if (shaders.fragmentShader != 0) {
			glDetachShader(program, shaders.fragmentShader);
		}
		return program;
	}
	else {
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());
		std::cerr << "Error al linkar el programa: " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

int main() {

	srand(static_cast<unsigned int>(time(NULL)));

	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Piramide 3D", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, Resize_Window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Error inicializando GLEW\n";
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Compilar shaders y crear programa
	ShaderProgram myProgram;
	myProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
	myProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

	GLuint program = CreateProgram(myProgram);
	glUseProgram(program);

	// Obtener uniforms
	GLint offsetLocation = glGetUniformLocation(program, "offset");
	GLint timeLocation = glGetUniformLocation(program, "time");

	//Crear piramide
	Pyramid pyramid;

	glm::vec2 offset = glm::vec2(0.6f, 0.0f);

	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		// Actualizar tiempo
		float timeValue = glfwGetTime();
		glUniform1f(timeLocation, timeValue);

		// Dibujar la piramide
		pyramid.Draw(offsetLocation, offset);

		glfwSwapBuffers(window);
	}

	glUseProgram(0);
	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}