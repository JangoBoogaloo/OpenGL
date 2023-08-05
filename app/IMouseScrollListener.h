#pragma once
#include <GLFW/glfw3.h>

class IMouseScrollListener
{
public:
    virtual void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;
};