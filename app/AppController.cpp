#include "AppController.h"

void AppController::OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, true);
    }
}

void AppController::OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}