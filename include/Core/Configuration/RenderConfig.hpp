#ifndef ESSENTIA_RENDERCONFIG_H
#define ESSENTIA_RENDERCONFIG_H

#include <string>

namespace Essentia
{
    struct RenderConfig {
        std::string renderMode = "PBR";
        bool bindlessTextures = true;
		bool vSync = false;
    };
}

#endif // !ESSENTIA_RENDERCONFIG_H