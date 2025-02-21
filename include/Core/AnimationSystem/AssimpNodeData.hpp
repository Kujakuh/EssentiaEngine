#ifndef ASSIMPNODE_H
#define ASSIMPNODE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Essentia
{
    struct AssimpNodeData
    {
        glm::mat4 transformation;
        std::string name;
        int childrenCount;
        std::vector<AssimpNodeData> children;
    };
}

#endif // !ASSIMPNODE_H