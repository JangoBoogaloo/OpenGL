#pragma once
#include <list>
#include "IMouseScrollListener.h"
#include "IKeyListener.h"

class Input
{
public:
    static std::vector<IMouseScrollListener*> MouseScrollListeners;
    static std::vector<IKeyListener*> KeyListeners;
    static void OnKeyBoardPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
        for (auto listener : KeyListeners) {
            listener->OnKeyBoardPressed(window, key, scancode, action, mods);
        }
    }

    static void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
        for (auto listener : MouseScrollListeners) {
            listener->OnMouseScroll(window, xoffset, yoffset);
        }
    }
};

std::vector<IMouseScrollListener*> Input::MouseScrollListeners;
std::vector<IKeyListener*> Input::KeyListeners;