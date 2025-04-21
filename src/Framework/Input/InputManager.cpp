#include <Framework/Input/InputManager.hpp>

namespace Essentia
{
    std::unordered_map<GLFWwindow*, InputManager*> InputManager::instances;
    InputManager* InputManager::activeInstance = nullptr;

    InputManager::InputManager(GLFWwindow* window) : window(window)
    {
        keyStates.clear();
        mouseButtonStates.clear();
        mouseData = {};

        glfwSetWindowUserPointer(window, this);
        glfwSetCursorPosCallback(window, MousePositionCallback);
        glfwSetScrollCallback(window, MouseScrollCallback);
    }

    void InputManager::Initialize(GLFWwindow* window)
    {
        if (instances.find(window) == instances.end()) 
            instances[window] = new InputManager(window);
        if (!activeInstance) 
            activeInstance = instances[window];
    }

    void InputManager::SetActiveInstance(GLFWwindow* window)
    {
        auto it = instances.find(window);
        if (it != instances.end()) activeInstance = it->second;
    }

    bool InputManager::IsKeyPressed(int key) { return GetKeyState(key) == PRESSED; }
    bool InputManager::IsKeyReleased(int key) { return GetKeyState(key) == RELEASED; }
    bool InputManager::IsKeyHeld(int key){ return GetKeyState(key) == HELD; }
    bool InputManager::IsMouseButtonPressed(int button) { return GetMouseButtonState(button) == PRESSED;}
    bool InputManager::IsMouseButtonReleased(int key) { return GetMouseButtonState(key) == RELEASED; }
    bool InputManager::IsMouseButtonHeld(int key) { return GetMouseButtonState(key) == HELD; }

    KEY_STATE InputManager::GetKeyState(int key)
    {
        if (activeInstance)
        {
            auto it = activeInstance->keyStates.find(key);
            if (it != activeInstance->keyStates.end()) return it->second;
        }
        return IDLE;
    }

    KEY_STATE InputManager::GetMouseButtonState(int button)
    {
        if (activeInstance)
        {
            auto it = activeInstance->mouseButtonStates.find(button);
            if (it != activeInstance->mouseButtonStates.end()) return it->second;
        }
        return IDLE;
    }

    const MouseData& InputManager::GetMouseData()
    {
        static MouseData defaultData;
        if (activeInstance) return activeInstance->mouseData;
        return defaultData; 
    }

    InputManager* InputManager::GetActiveInstance() {return activeInstance;}

    void InputManager::Update()
    {
        // Teclas
        for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key)
        {
            int state = glfwGetKey(window, key);
            KEY_STATE prev = prevKeyStates[key];

            if (state == GLFW_PRESS)
            {
                if (prev == PRESSED || prev == HELD)
                    keyStates[key] = HELD;
                else
                    keyStates[key] = PRESSED;
            }
            else if (state == GLFW_RELEASE)
            {
                if (prev == PRESSED || prev == HELD)
                    keyStates[key] = RELEASED;
                else
                    keyStates[key] = IDLE;
            }

            prevKeyStates[key] = keyStates[key];
        }

        // Botones del ratón
        for (int button = GLFW_MOUSE_BUTTON_1; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
        {
            int state = glfwGetMouseButton(window, button);
            KEY_STATE prev = prevMouseButtonStates[button];

            if (state == GLFW_PRESS)
            {
                if (prev == PRESSED || prev == HELD)
                    mouseButtonStates[button] = HELD;
                else
                    mouseButtonStates[button] = PRESSED;
            }
            else if (state == GLFW_RELEASE)
            {
                if (prev == PRESSED || prev == HELD)
                    mouseButtonStates[button] = RELEASED;
                else
                    mouseButtonStates[button] = IDLE;
            }

            prevMouseButtonStates[button] = mouseButtonStates[button];
        }
    }

    // Callbacks de GLFW
    void InputManager::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        auto it = instances.find(window);
        if (it != instances.end())
        {
            it->second->mouseData.x = xpos;
            it->second->mouseData.y = ypos;
        }
    }
    void InputManager::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        auto it = instances.find(window);
        if (it != instances.end())
        {
            it->second->mouseData.scrollX = xoffset;
            it->second->mouseData.scrollY = yoffset;
        }
    }

}
