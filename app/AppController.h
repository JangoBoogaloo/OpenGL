#pragma once
#include "IKeyListener.h"
class AppController : public IKeyListener
{
public:
    void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
};

