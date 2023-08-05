#pragma once
#include "IKeyListener.h"
#include "IFrameBufferSizeListener.h"

class AppController : 
    public IKeyListener,
    public IFrameBufferSizeListener
{
public:
    void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
};

