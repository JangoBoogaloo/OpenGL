#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLErrorCheck.h"
#include "Triangle.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

const int DefaultWidth = 1920;
const int DefaultHeight = 1080;
const unsigned int ElementPerVertex = 3;

float roll = 0;
float yaw = 0;
float pitch = 0;

auto camera = Camera();
glm::vec3 eye(0, 0, 1);
glm::vec3 center(0, 0, 0);
glm::vec3 upDirection(0, 1, 0);
const auto fovYDegree = 60.0f, zNear = 0.1f, zFar = 99.0f;

static void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (action == GLFW_RELEASE) return;
    switch (key) {
    case GLFW_KEY_Y:
        yaw += 1.0f;
        break;
    case GLFW_KEY_R:
        roll += 1.0f;
        break;
    case GLFW_KEY_P:
        pitch += 1.0f;
        break;
    case GLFW_KEY_SPACE:
        yaw = 0;
        pitch = 0;
        roll = 0;
        break;
    }
}

static void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    eye.z += yoffset;
    eye.z = fmax(1, eye.z);
    camera.LookAt(eye, center, upDirection);
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
        std::cout << "Loaded file: " << filePath << std::endl;
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

static unsigned int OnLoadTexture(int width, int height, int nrChannel, const unsigned char* data)
{
    unsigned int textureID;
    GL_EXEC(glGenTextures(1, &textureID));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureID));
    // set the texture wrapping/filtering options (on the currently bound texture object)
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    //When scale down, make it more blocked pattern
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //When scale up, make it more linear pattern
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GL_EXEC(glGenerateMipmap(GL_TEXTURE_2D));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));
    return textureID;
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
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChanged);
    glfwSetKeyCallback(window, OnKeyBoardPressed);
    glfwSetScrollCallback(window, OnMouseScroll);

    glViewport(0, 0, DefaultWidth, DefaultHeight);

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
        const auto vertexProgram = LoadFileString("Shaders\\mvp.vert");
        const auto fragmentProgram = LoadFileString("Shaders\\ndc.frag");
        auto texture = Texture("Textures\\coordinate.jpg");
        texture.Load(OnLoadTexture);
        auto ndcShader = Shader(vertexProgram.c_str(), fragmentProgram.c_str());

        const auto vertexAttributeLocation = ndcShader.GetAttributeLocation("inPosition");
        const auto VertexStride = ElementPerVertex * sizeof(float);
        const auto VertexOffsetPointer = (void*)0;

        auto model = Model();
        model.Scale(1, 1, 1);
        camera.PerspectiveProjection(fovYDegree, ((float)DefaultWidth) / ((float)DefaultHeight), zNear, zFar);
        camera.LookAt(eye, center, upDirection);

        // vao[location] <- vbo[0]
        GL_EXEC(glVertexAttribPointer(vertexAttributeLocation, ElementPerVertex, GL_FLOAT, GL_FALSE, VertexStride, VertexOffsetPointer));
        //The reason why the fuck we need this: https://www.gamedev.net/forums/topic/655785-is-glenablevertexattribarray-redundant/
        GL_EXEC(glEnableVertexAttribArray(vertexAttributeLocation));

        while (!glfwWindowShouldClose(window))
        {
            model.Rotate(yaw, pitch, roll);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ndcShader.Use();
            ndcShader.SetUniformMatrix4fv("model", glm::transpose(model.GetModelMatrix()));
            ndcShader.SetUniformMatrix4fv("view", glm::transpose(camera.GetView()));
            ndcShader.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjection()));
            GL_EXEC(glBindVertexArray(vao));
            GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, TriangleShapeVertices.size()));
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
