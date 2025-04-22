#ifndef ESSENTIA_WINDOWCONFIG_H
#define ESSENTIA_WINDOWCONFIG_H

#include <string>

namespace Essentia
{
    struct WindowConfig {
        std::string title = "Essentia";
        int samples = 8;  // MSAA
        int aspectRatioWidth = 16;
        int aspectRatioHeight = 9;
        int width = 900;
        int height = width * (aspectRatioHeight/aspectRatioWidth);
    };
}

#endif // !ESSENTIA_WINDOWCONFIG_H