#include <Core/Graphics/Mesh.hpp>

namespace Essentia
{
    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(LOC_POSITION);
        glVertexAttribPointer(LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(LOC_NORMAL);
        glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(LOC_TEX_COORD);
        glVertexAttribPointer(LOC_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(LOC_TANGENT);
        glVertexAttribPointer(LOC_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(LOC_BITANGENT);
        glVertexAttribPointer(LOC_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glEnableVertexAttribArray(LOC_IDS);
        glVertexAttribIPointer(LOC_IDS, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(LOC_WEIGHTS);
        glVertexAttribPointer(LOC_WEIGHTS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindVertexArray(0);
    }

    void Mesh::bindTextures()
    {
        for (const auto& pair : textures)
        {
            const std::string& uniformName = pair.first;
            std::shared_ptr<Texture> texture = pair.second;

            if (GLAD_GL_ARB_bindless_texture && bindlessTexturesMode)
                shader->setUniform(uniformName.c_str(), texture->getHandle());
            else
            {
                texture->bind();
                shader->setUniform(uniformName.c_str(), texture->getTextureUnit());
            }
        }
    }

    void Mesh::unbindTextures()
    {
        for (const auto& pair : textures) pair.second->unbind();
    }

    void Mesh::updateVertices(const std::vector<Vertex>& newVertices)
    {
        vertices = newVertices;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    }

    void Mesh::render()
    {
        glBindVertexArray(VAO);
        bindTextures();

        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        unbindTextures();
    }
}