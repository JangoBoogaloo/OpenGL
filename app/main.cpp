#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Rectangle.h"
#include "Shader.h"
#include "ModelMatrix.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Input.h"
#include "AppController.h"
#include "Model.h"

const int DefaultWidth = 1920;
const int DefaultHeight = 1080;

const unsigned int ElementPerVertex = 3;
const unsigned int ElementPerTex = 2;

const auto fovYDegree = 60.0f, zNear = 0.1f, zFar = 99.0f;

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

static void RenderLoop(GLFWwindow* window);

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
    auto appController = AppController();
    Input::KeyListeners.push_back(&appController);
    Input::FrameBufferSizeListeners.push_back(&appController);
    glfwSetFramebufferSizeCallback(window, Input::OnFrameBufferSizeChanged);
    glfwSetKeyCallback(window, Input::OnKey);
    glfwSetScrollCallback(window, Input::OnMouseScroll);
    RenderLoop(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

static void RenderLoop(GLFWwindow* window) {
    auto modelMatrix = Matrix::ModelMatrix();
    auto camera = Matrix::Camera();
    camera.PerspectiveProjection(fovYDegree, ((float)DefaultWidth) / ((float)DefaultHeight), zNear, zFar);
    Input::KeyListeners.push_back(&modelMatrix);
    Input::MouseScrollListeners.push_back(&camera);

    GL_EXEC(glViewport(0, 0, DefaultWidth, DefaultHeight));

    //Vertex buffer object
    unsigned int vao;
    GL_EXEC(glGenVertexArrays(1, &vao));
    unsigned int vbo;
    GL_EXEC(glGenBuffers(1, &vbo));

    // 2. copy our vertices array in a buffer for OpenGL to use
    GL_EXEC(glBindVertexArray(vao));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    const unsigned int vertexBufferSize = RectangleShapeVertices.size() * sizeof(float);
    GL_EXEC(glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, RectangleShapeVertices.data(), GL_STATIC_DRAW));

    // 3 generate element buffer object, load into OpenGL to use
    unsigned int ebo;
    GL_EXEC(glGenBuffers(1, &ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RectangleIndices.size() * sizeof(unsigned int), RectangleIndices.data(), GL_STATIC_DRAW);

    auto modelFile = "..\\Models\\teapot.obj";
    auto model = Model(modelFile);
    const auto vertexProgram = LoadFileString("Shaders\\Color.vert");
    const auto fragmentProgram = LoadFileString("Shaders\\Color.frag");
    auto shader = Shader(vertexProgram.c_str(), fragmentProgram.c_str());

    const auto vertexAttributeLocation = shader.GetAttributeLocation("inPosition");
    model.LoadToGPU(vertexAttributeLocation, -1, -1);

    while (!glfwWindowShouldClose(window))
    {
        GL_EXEC(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
        GL_EXEC(glClear(GL_COLOR_BUFFER_BIT));
        shader.Use();
        shader.SetUniformMatrix4fv("model", glm::transpose(modelMatrix.GetModelMatrix()));
        shader.SetUniformMatrix4fv("view", glm::transpose(camera.GetView()));
        shader.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjection()));
        model.Draw(shader);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}