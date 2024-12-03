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

    void Mesh::updateMaterial()
    {
        if (!shader) {
            throw std::runtime_error("No shader assigned to material.");
        }

        shader->setUniform("material.color", material.color);
        shader->setUniform("material.shininess", material.shininess);

        if (GLAD_GL_ARB_bindless_texture && bindlessTexturesMode) {
            if (material.diffuse != nullptr) shader->setUniform("material.specular", material.specular->getHandle());
            if (material.specular != nullptr) shader->setUniform("material.normal", material.normal->getHandle());
            if (material.normal != nullptr) shader->setUniform("material.height", material.height->getHandle());
            if (material.height != nullptr) ;shader->setUniform("material.diffuse", material.diffuse->getHandle());
        }
        else 
        {
            if (material.diffuse != nullptr)
            {
                material.diffuse->bind();
                shader->setUniform("material.diffuse", material.diffuse->getTextureUnit());
                material.height->unbind();
            }
            if (material.specular != nullptr)
            {
                material.specular->bind();
                shader->setUniform("material.specular", material.specular->getTextureUnit());
                material.normal->unbind();
            }
            if (material.normal != nullptr)
            {
                material.normal->bind();
                shader->setUniform("material.normal", material.normal->getTextureUnit());
                material.specular->unbind();
            }
            if (material.height != nullptr)
            {
                material.height->bind();
                shader->setUniform("material.height", material.height->getTextureUnit());
                material.diffuse->unbind();
            }
        }
    }

    void Mesh::bindMaterial() {
        if (!GLAD_GL_ARB_bindless_texture || !bindlessTexturesMode) {
            if (material.diffuse != nullptr) material.diffuse->bind();
            if (material.specular != nullptr) material.specular->bind();
            if (material.normal != nullptr) material.normal->bind();
            if (material.height != nullptr) material.height->bind();
        }
    }

    void Mesh::unbindMaterial() {
        if (!GLAD_GL_ARB_bindless_texture || !bindlessTexturesMode) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Mesh::SetMaterial(const Material& newMaterial) {
        material = newMaterial;
    }

    std::weak_ptr<Material> Mesh::GetMaterial() const {
        return std::make_shared<Material>(material);
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

        if (needsUpdate) {
            updateMaterial();
            needsUpdate = false;
        }

        bindMaterial();

        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        unbindMaterial();
    }
}