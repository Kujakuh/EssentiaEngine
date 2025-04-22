#include <Core/Configuration/AppConfig.hpp>

namespace Essentia
{
    // Window configuration methods
    AppConfig& AppConfig::setWindowSize(int width) {
        if (width <= 0) {
            throw std::invalid_argument("Window dimensions must be positive");
        }
        windowConfig.width = width;
		float ratio = static_cast<float>(windowConfig.aspectRatioHeight) / static_cast<float>(windowConfig.aspectRatioWidth);
        windowConfig.height = static_cast<int>(width * ratio);
        return *this;
    }

    AppConfig& AppConfig::setWindowTitle(const std::string& title) {
        windowConfig.title = title;
        return *this;
    }

    AppConfig& AppConfig::setMSAASamples(int samples) {
        if (samples < 1 || samples > 16) {
            throw std::invalid_argument("MSAA samples must be between 1 and 16");
        }
        windowConfig.samples = samples;
        return *this;
    }

    AppConfig& AppConfig::setAspectRatio(int width, int height) {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Aspect ratio values must be positive");
        }
        windowConfig.aspectRatioWidth = width;
        windowConfig.aspectRatioHeight = height;
        return *this;
    }

    // OpenGL configuration methods
    AppConfig& AppConfig::enableDepthTest(bool enable) {
        openGLConfig.depthTest = enable;
        return *this;
    }

    AppConfig& AppConfig::enableBlending(bool enable, GLenum srcFactor, GLenum destFactor) {
        openGLConfig.blend = enable;
        openGLConfig.blendSrc = srcFactor;
        openGLConfig.blendDest = destFactor;
        return *this;
    }

    AppConfig& AppConfig::setGLVersion(int major, int minor) {
        if (major < 3 || (major == 3 && minor < 3)) {
            throw std::invalid_argument("OpenGL version must be at least 3.3");
        }
        openGLConfig.glMajorVersion = major;
        openGLConfig.glMinorVersion = minor;
        return *this;
    }

    AppConfig& AppConfig::setWireframeMode(bool enable) {
        openGLConfig.wireframeMode = enable;
        return *this;
    }

    AppConfig& AppConfig::enableDebugMode(bool enable) {
        openGLConfig.debugMode = enable;
        return *this;
    }

    void AppConfig::setupFramebufferCallback(GLFWwindow* window, GLFWframebuffersizefun callback) const {
        if (!window) {
            throw std::invalid_argument("Window handle is null");
        }
        glfwSetFramebufferSizeCallback(window, callback);
    }

    // Render configuration methods
    AppConfig& AppConfig::setRenderMode(const std::string& mode) {
        if (mode != "PBR" && mode != "Phong") {
            throw std::invalid_argument("Invalid render mode. Supported: PBR, Phong");
        }
        renderConfig.renderMode = mode;
        return *this;
    }

    AppConfig& AppConfig::setBindlessTextures(bool enable) {
        renderConfig.bindlessTextures = enable;
        return *this;
    }


    void glfwErrorCallback(int error, const char* description) {
        std::cerr << " ERROR::GLFW::" << error << "::[" << description << ']\n';
    }

    GLFWwindow* AppConfig::createWindow() const {

		if (openGLConfig.debugMode) {
			glfwSetErrorCallback(glfwErrorCallback);
		}

        // Set all window hints based on configuration
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLConfig.glMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLConfig.glMinorVersion);

        if (openGLConfig.coreProfile) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        if (openGLConfig.debugMode) {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        }

        glfwWindowHint(GLFW_SAMPLES, windowConfig.samples);

        // Create the window
        GLFWwindow* window = window = glfwCreateWindow(
            windowConfig.width,
            windowConfig.height,
            windowConfig.title.c_str(),
            nullptr,
            nullptr
        );

        if (!window)
        {
            std::cout << " ERROR::GLFW::WINDOW::CREATE" << '\n';
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwSetWindowAspectRatio(window, windowConfig.aspectRatioWidth, windowConfig.aspectRatioHeight);

        return window;
    }
}

