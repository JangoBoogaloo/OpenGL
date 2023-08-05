#pragma once
#include <vector>
#include "IMouseScrollListener.h"
#include "IKeyListener.h"
#include "IFrameBufferSizeListener.h"

class Input
{
public:
    static std::vector<IMouseScrollListener*> MouseScrollListeners;
    static std::vector<IKeyListener*> KeyListeners;
    static std::vector<IFrameBufferSizeListener*> FrameBufferSizeListeners;

    static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        for (auto listener : KeyListeners) {
            listener->OnKey(window, key, scancode, action, mods);
        }
    }

    static void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
        for (auto listener : MouseScrollListeners) {
            listener->OnMouseScroll(window, xoffset, yoffset);
        }
    }

    static void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
    {
        for (auto listener : FrameBufferSizeListeners) {
            listener->OnFrameBufferSizeChanged(window, width, height);
        }
    }
};

std::vector<IMouseScrollListener*> Input::MouseScrollListeners;
std::vector<IKeyListener*> Input::KeyListeners;
std::vector<IFrameBufferSizeListener*> Input::FrameBufferSizeListeners;