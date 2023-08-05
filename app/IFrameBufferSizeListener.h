#pragma once
#include "GLHeaders.h"

class IFrameBufferSizeListener
{
public:
    virtual void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height) = 0;
};

