#include <glm/glm.hpp>

namespace Essentia
{
    struct BoneInfo
    {
        // Index in finalBoneMatrices
        int id;

        // Offset matrix transforms vertex from model space to bone space
        glm::mat4 offset;

    };
}