#ifndef ESSENTIA_CONFIG_LOADER_H
#define ESSENTIA_CONFIG_LOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <Core/Configuration/AppConfig.hpp>
#include <Debug/openglDebug.hpp>

namespace Essentia
{
    class ConfigLoader
    {
        public:
            explicit ConfigLoader(const AppConfig& config);
            AppConfig config;

            GLFWwindow* createWindow();

            void initializeOpenGL(GLFWwindow* window) const;

            void setupFramebufferCallback(GLFWwindow* window, GLFWframebuffersizefun callback) const;

            void applyViewport(GLFWwindow* window) const;
            void applyDepthTest() const;
            void applyBlending() const;
            void applyMultisample() const;
            void applyDebugOutput(GLDEBUGPROC callback = glDebugOutput) const;
            //void applyInputMode(GLFWwindow* window) const;
            void applyVSync() const;

    };
}

#endif // ESSENTIA_CONFIG_LOADER_H
