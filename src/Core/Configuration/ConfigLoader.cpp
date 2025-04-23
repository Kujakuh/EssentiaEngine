#include <Core/Configuration/ConfigLoader.hpp>

namespace Essentia
{
    ConfigLoader::ConfigLoader(const AppConfig& config) : config(config) {}

    GLFWwindow* ConfigLoader::createWindow()
    {
        if (config.getOpenGLConfig().debugMode) {
            glfwSetErrorCallback([](int error, const char* description) {
                std::cerr << "GLFW ERROR " << error << ": " << description << "\n";
                });
        }

        const auto& gl = config.getOpenGLConfig();
        const auto& win = config.getWindowConfig();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl.glMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl.glMinorVersion);

        if (gl.coreProfile)
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (gl.debugMode)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        glfwWindowHint(GLFW_SAMPLES, win.samples);

        GLFWwindow* window = glfwCreateWindow(win.width, win.height, win.title.c_str(), NULL, NULL);

        if (!window)
        {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            throw std::runtime_error("GLFW window creation failed");
        }

        glfwSetWindowAspectRatio(window, win.aspectRatioWidth, win.aspectRatioHeight);

        return window;
    }

    void ConfigLoader::initializeOpenGL(GLFWwindow* window) const
    {
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "ERROR::GLAD::INIT" << '\n';
            glfwTerminate();
            return;
        }

		applyViewport(window);
		setupFramebufferCallback(window, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			});
		applyDepthTest();
		applyBlending();
		applyMultisample();
		applyDebugOutput();
		//applyInputMode(window); y wireframeMode
		applyVSync();

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    }

    void ConfigLoader::setupFramebufferCallback(GLFWwindow* window, GLFWframebuffersizefun callback) const 
    {
        if (!window) throw std::invalid_argument("Window handle is null");
        glfwSetFramebufferSizeCallback(window, callback);
    }

    void ConfigLoader::applyViewport(GLFWwindow* window) const
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    }

    void ConfigLoader::applyDepthTest() const
    {
        if (config.getOpenGLConfig().depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void ConfigLoader::applyBlending() const
    {
        if (config.getOpenGLConfig().blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(config.getOpenGLConfig().blendSrc, config.getOpenGLConfig().blendDest);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void ConfigLoader::applyMultisample() const
    {
        if (config.getWindowConfig().samples > 0)
        {
            glEnable(GL_MULTISAMPLE);
            glfwWindowHint(GLFW_SAMPLES, config.getWindowConfig().samples);
        }
        else
            glDisable(GL_MULTISAMPLE);
    }

    void ConfigLoader::applyDebugOutput(GLDEBUGPROC callback) const
    {
        if (config.getOpenGLConfig().debugMode)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(callback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
        else
        {
            glDisable(GL_DEBUG_OUTPUT);
        }
    }

    //void ConfigLoader::applyInputMode(GLFWwindow* window) const
    //{
    //    glfwSetInputMode(window, GLFW_CURSOR, config.getOpenGLConfig);
    //}

    void ConfigLoader::applyVSync() const
    {
        glfwSwapInterval(config.getRenderConfig().vSync ? 1 : 0);
    }

    /*void ConfigLoader::applyRuntimeConfig(GLFWwindow* window) const
    {
        const auto& gl = config.getOpenGLConfig();
        const auto& render = config.getRenderConfig();

        if (gl.depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        if (gl.blend) {
            glEnable(GL_BLEND);
            glBlendFunc(gl.blendSrc, gl.blendDest);
        }
        else {
            glDisable(GL_BLEND);
        }

        if (gl.wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapInterval(render.vSync ? 1 : 0);
    }*/

}
