#pragma once

#include "GLHeaders.h"

class IKeyListener
{
public:
    virtual void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};