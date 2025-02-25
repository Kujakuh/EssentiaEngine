#ifndef BONEINFO_H
#define BONEINFO_H

#include <glm/glm.hpp>

namespace Essentia
{
    struct BoneInfo
    {
        int id;
        // Offset matrix transforms vertex from model space to bone space
        glm::mat4 offset;
    };
}

#endif // !BONEINFO_H