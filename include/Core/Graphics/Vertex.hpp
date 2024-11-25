#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <Core/values.hpp>

namespace Essentia
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];

        Vertex(const glm::vec3& position)
            : Position(position), Normal(glm::vec3(0.0f, 0.0f, 1.0f)),
            TexCoords(glm::vec2(0.0f, 0.0f)), Tangent(glm::vec3(1.0f, 0.0f, 0.0f)),
            Bitangent(glm::vec3(0.0f, 1.0f, 0.0f))
        {
            std::fill(std::begin(m_BoneIDs), std::end(m_BoneIDs), 0);
            std::fill(std::begin(m_Weights), std::end(m_Weights), 0.0f);
        }

        Vertex(const glm::vec3& position, const glm::vec2& texCoords)
            : Position(position), Normal(glm::vec3(0.0f, 0.0f, 1.0f)),
            TexCoords(texCoords), Tangent(glm::vec3(1.0f, 0.0f, 0.0f)),
            Bitangent(glm::vec3(0.0f, 1.0f, 0.0f))
        {
            std::fill(std::begin(m_BoneIDs), std::end(m_BoneIDs), 0);
            std::fill(std::begin(m_Weights), std::end(m_Weights), 0.0f);
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
            : Position(position), Normal(normal), TexCoords(texCoords),
            Tangent(glm::vec3(1.0f, 0.0f, 0.0f)), Bitangent(glm::vec3(0.0f, 1.0f, 0.0f))
        {
            std::fill(std::begin(m_BoneIDs), std::end(m_BoneIDs), 0);
            std::fill(std::begin(m_Weights), std::end(m_Weights), 0.0f);
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords,
            const glm::vec3& tangent)
            : Position(position), Normal(normal), TexCoords(texCoords),
            Tangent(tangent), Bitangent(glm::vec3(0.0f, 1.0f, 0.0f))
        {
            std::fill(std::begin(m_BoneIDs), std::end(m_BoneIDs), 0);
            std::fill(std::begin(m_Weights), std::end(m_Weights), 0.0f);
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords,
            const glm::vec3& tangent, const glm::vec3& bitangent)
            : Position(position), Normal(normal), TexCoords(texCoords),
            Tangent(tangent), Bitangent(bitangent)
        {
            std::fill(std::begin(m_BoneIDs), std::end(m_BoneIDs), 0);
            std::fill(std::begin(m_Weights), std::end(m_Weights), 0.0f);
        }

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords,
            const glm::vec3& tangent, const glm::vec3& bitangent, const int boneIDs[MAX_BONE_INFLUENCE], const float weights[MAX_BONE_INFLUENCE])
            : Position(position), Normal(normal), TexCoords(texCoords),
            Tangent(tangent), Bitangent(bitangent)
        {
            for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
                m_BoneIDs[i] = boneIDs[i];
                m_Weights[i] = weights[i];
            }
        }
    };
}

#endif // !VERTEX_H