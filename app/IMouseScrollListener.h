#pragma once
#include "GLHeaders.h"

class IMouseScrollListener
{
public:
    virtual void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) = 0;
};