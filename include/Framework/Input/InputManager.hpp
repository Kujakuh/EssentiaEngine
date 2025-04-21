#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>

#include <unordered_map>

#include <Shared/enums.hpp>
#include <Shared/key_input_values.hpp>
#include <Framework/Input/MouseData.hpp>

namespace Essentia 
{
    class InputManager
    {
        public:
            static void Initialize(GLFWwindow* window);
            static void SetActiveInstance(GLFWwindow* window);

            static KEY_STATE GetKeyState(int key);
            static bool IsKeyPressed(int key);
            static bool IsKeyReleased(int key);
            static bool IsKeyHeld(int key);

            static KEY_STATE GetMouseButtonState(int button);
            static bool IsMouseButtonPressed(int button);
            static bool IsMouseButtonReleased(int key);
            static bool IsMouseButtonHeld(int key);

            static const MouseData& GetMouseData();

            static InputManager* GetActiveInstance();
            void Update();

        private:

            InputManager(GLFWwindow* window);

            // GLFW Callbacks
            static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
            static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

            static std::unordered_map<GLFWwindow*, InputManager*> instances;
            static InputManager* activeInstance;

            GLFWwindow* window;
            std::unordered_map<int, KEY_STATE> keyStates;
            std::unordered_map<int, KEY_STATE> mouseButtonStates;
            MouseData mouseData;
            std::unordered_map<int, KEY_STATE> prevKeyStates;
            std::unordered_map<int, KEY_STATE> prevMouseButtonStates;

    };
}

#endif // !INPUTMANAGER_H