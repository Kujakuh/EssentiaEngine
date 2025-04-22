#ifndef ESSENTIA_OPENGLCONFIG_H
#define ESSENTIA_OPENGLCONFIG_H

#include <glad/glad.h>

namespace Essentia
{
    struct OpenGLConfig {
        bool depthTest = true;
        bool blend = true;
        GLenum blendSrc = GL_SRC_ALPHA;
        GLenum blendDest = GL_ONE_MINUS_SRC_ALPHA;
        bool wireframeMode = false;
		bool debugMode = true;
        int glMajorVersion = 4;
        int glMinorVersion = 3;
        bool coreProfile = true;
    };
}

#endif // !ESSENTIA_OPENGLCONFIG_H