#ifndef ESSENTIA_CONFIG_H
#define ESSENTIA_CONFIG_H

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "WindowConfig.hpp"
#include "OpenGLConfig.hpp"
#include "RenderConfig.hpp"

namespace Essentia
{
    class AppConfig
    {
        public:

            AppConfig() = default;

            // Window configuration
            AppConfig& setWindowSize(int width);
            AppConfig& setWindowTitle(const std::string& title);
            AppConfig& setMSAASamples(int samples);
            AppConfig& setAspectRatio(int width, int height);

            // OpenGL configuration
            AppConfig& enableDepthTest(bool enable);
            AppConfig& setGLVersion(int major, int minor);
            AppConfig& enableBlending(bool enable, GLenum srcFactor, GLenum destFactor);
            AppConfig& setWireframeMode(bool enable);
            AppConfig& enableDebugMode(bool enable);

            // Render configuration
            AppConfig& setRenderMode(const std::string& mode);
            AppConfig& setBindlessTextures(bool enable);

            const WindowConfig& getWindowConfig() const { return windowConfig; }
            const OpenGLConfig& getOpenGLConfig() const { return openGLConfig; }
            const RenderConfig& getRenderConfig() const { return renderConfig; }

            GLFWwindow* createWindow() const;

        private:
            WindowConfig windowConfig;
            OpenGLConfig openGLConfig;
            RenderConfig renderConfig;
        };
}

#endif // !ESSENTIA_CONFIG_H