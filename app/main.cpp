#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Triangle.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Input.h"
#include "AppController.h"

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
    auto model = Model();
    auto appController = AppController();
    auto camera = Camera();
    camera.PerspectiveProjection(fovYDegree, ((float)DefaultWidth) / ((float)DefaultHeight), zNear, zFar);

    Input::KeyListeners.push_back(&appController);
    Input::FrameBufferSizeListeners.push_back(&appController);
    Input::KeyListeners.push_back(&model);
    Input::MouseScrollListeners.push_back(&camera);

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

    // 3 generate element buffer object, load into OpenGL to use
    unsigned int ebo;
    GL_EXEC(glGenBuffers(1, &ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, TriangleIndices.size() * sizeof(unsigned int), TriangleIndices.data(), GL_STATIC_DRAW);

    {
        const auto vertexProgram = LoadFileString("Shaders\\mvp.vert");
        const auto fragmentProgram = LoadFileString("Shaders\\ndc.frag");
        auto texture = Texture("Textures\\coordinate.jpg");
        const auto textureId = texture.Load(OnLoadTexture);

        auto ndcShader = Shader(vertexProgram.c_str(), fragmentProgram.c_str());

        const auto vertexAttributeLocation = ndcShader.GetAttributeLocation("inPosition");
        const auto VertexTexStride = (ElementPerVertex + ElementPerTex) * sizeof(float);
        const auto VertexOffsetPointer = (void*)0;

        // vao[location] <- vbo[0]
        GL_EXEC(glVertexAttribPointer(vertexAttributeLocation, ElementPerVertex, GL_FLOAT, GL_FALSE, VertexTexStride, VertexOffsetPointer));
        GL_EXEC(glEnableVertexAttribArray(vertexAttributeLocation));

        const unsigned int TextureLayoutLocation = ndcShader.GetAttributeLocation("inTexCoord");
        const unsigned int VertexStride = ElementPerVertex * sizeof(float);
        const void* const TextureOffsetPointer = (void*)VertexStride;

        GL_EXEC(glVertexAttribPointer(TextureLayoutLocation, ElementPerTex, GL_FLOAT, GL_FALSE, VertexTexStride, TextureOffsetPointer));
        GL_EXEC(glEnableVertexAttribArray(TextureLayoutLocation));

        glfwSetFramebufferSizeCallback(window, Input::OnFrameBufferSizeChanged);
        glfwSetKeyCallback(window, Input::OnKeyBoardPressed);
        glfwSetScrollCallback(window, Input::OnMouseScroll);
        glActiveTexture(GL_TEXTURE0);
        GL_EXEC(glBindTexture(GL_TEXTURE_2D, textureId));
        while (!glfwWindowShouldClose(window))
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ndcShader.Use();
            ndcShader.SetUniformMatrix4fv("model", glm::transpose(model.GetModelMatrix()));
            ndcShader.SetUniformMatrix4fv("view", glm::transpose(camera.GetView()));
            ndcShader.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjection()));
            GL_EXEC(glBindVertexArray(vao));
            GL_EXEC(glDrawElements(GL_TRIANGLES, TriangleIndices.size(), GL_UNSIGNED_INT, 0));
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        GL_EXEC(glDeleteVertexArrays(1, &vao));
        GL_EXEC(glDeleteBuffers(1, &vbo));
        GL_EXEC(glDeleteBuffers(1, &ebo));
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}
