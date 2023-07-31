#include <iostream>
#include <fstream>
#include<sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLErrorCheck.h"
#include "Triangle.h"
#include "Shader.h"

const int DefaultWidth = 1920;
const int DefaultHeight = 1080;
const unsigned int ElementPerVertex = 3;

const char* const VertexProgram =
"#version 460 core\n"
"layout(location = 0) in vec3 inPosition;"
"void main() {"
"   gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);"
"}";

const char* const FragmentProgram =
"#version 460 core\n"
"out vec4 FragColor;"
"void main() {"
"   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);"
"}";

static void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void OnEscapeKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
    }
}

static const std::string LoadFileString(const char* filePath)
{
    std::ifstream fileStream;

    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        fileStream.open(filePath);

        std::stringstream fileStringStream;
        // read file's buffer contents into streams
        fileStringStream << fileStream.rdbuf();
        // close file handlers
        fileStream.close();
        return fileStringStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cerr << "FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
        std::cerr << e.what() << std::endl;
        DEBUG_THROW;
        return std::string();
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(DefaultWidth, DefaultHeight, "App", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, DefaultWidth, DefaultHeight);
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChanged);
    glfwSetKeyCallback(window, OnEscapeKeyPressed);


    //Vertex buffer object
    unsigned int vao;
    GL_EXEC(glGenVertexArrays(1, &vao));
    unsigned int vbo;
    GL_EXEC(glGenBuffers(1, &vbo));

    // 2. copy our vertices array in a buffer for OpenGL to use
    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    const unsigned int vertexBufferSize = TriangleShapeVertices.size() * sizeof(float);
    GL_EXEC(glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, TriangleShapeVertices.data(), GL_STATIC_DRAW));

    {

        const auto vertexProgram = LoadFileString("Shaders\\ndc.vert");
        const auto fragmentProgram = LoadFileString("Shaders\\ndc.frag");
        auto ndcShader = Shader(vertexProgram.c_str(), fragmentProgram.c_str());

        const auto vertexAttributeLocation = ndcShader.GetAttributeLocation("inPosition");
        const auto VertexStride = ElementPerVertex * sizeof(float);
        const auto VertexOffsetPointer = (void*)0;

        // vao[location] <- vbo[0]
        GL_EXEC(glVertexAttribPointer(vertexAttributeLocation, ElementPerVertex, GL_FLOAT, GL_FALSE, VertexStride, VertexOffsetPointer));
        //The reason why the fuck we need this: https://www.gamedev.net/forums/topic/655785-is-glenablevertexattribarray-redundant/
        GL_EXEC(glEnableVertexAttribArray(vertexAttributeLocation));

        while (!glfwWindowShouldClose(window))
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ndcShader.Use();
            GL_EXEC(glBindVertexArray(vao));
            GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, TriangleShapeVertices.size()));
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
