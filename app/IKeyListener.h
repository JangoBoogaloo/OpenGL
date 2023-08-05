#pragma once

#include <GLFW/glfw3.h>

class IKeyListener
{
public:
    virtual void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};