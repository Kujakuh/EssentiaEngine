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

    void Mesh::updateMaterial(int i)
    {
        if (!shader) {
            throw std::runtime_error("No shader assigned to material.");
        }
        if (!materials.empty())
        {
            shader->setUniform("material.color", materials[i].color);
            shader->setUniform("material.shininess", materials[i].shininess);

            if (GLAD_GL_ARB_bindless_texture && bindlessTexturesMode) {
                if (materials[i].diffuse != nullptr) shader->setUniform("material.diffuse", materials[i].diffuse->getHandle());
                if (materials[i].specular != nullptr) shader->setUniform("material.specular", materials[i].specular->getHandle());
                if (materials[i].normal != nullptr)  shader->setUniform("material.normal", materials[i].normal->getHandle());
                if (materials[i].height != nullptr) shader->setUniform("material.height", materials[i].height->getHandle());
            }
            else
            {
                if (materials[i].diffuse != nullptr)
                {
                    materials[i].diffuse->bind();
                    shader->setUniform("material.diffuse", materials[i].diffuse->getTextureUnit());
                    materials[i].diffuse->unbind();
                }
                if (materials[i].specular != nullptr)
                {
                    materials[i].specular->bind();
                    shader->setUniform("material.specular", materials[i].specular->getTextureUnit());
                    materials[i].specular->unbind();
                }
                if (materials[i].normal != nullptr)
                {
                    materials[i].normal->bind();
                    shader->setUniform("material.normal", materials[i].normal->getTextureUnit());
                    materials[i].normal->unbind();
                }
                if (materials[i].height != nullptr)
                {
                    materials[i].height->bind();
                    shader->setUniform("material.height", materials[i].height->getTextureUnit());
                    materials[i].height->unbind();
                }
            }
        }
        //else shader->setUniform("material.color", glm::vec3(1.0f));
    }

    void Mesh::bindMaterial(int i) {
        if (!GLAD_GL_ARB_bindless_texture || !bindlessTexturesMode) {
            if (materials[i].diffuse != nullptr) materials[i].diffuse->bind();
            if (materials[i].specular != nullptr) materials[i].specular->bind();
            if (materials[i].normal != nullptr) materials[i].normal->bind();
            if (materials[i].height != nullptr) materials[i].height->bind();
        }
    }

    void Mesh::unbindMaterial() {
        if (!GLAD_GL_ARB_bindless_texture || !bindlessTexturesMode) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void Mesh::SetMaterial(const Material& newMaterial, int i) {
        materials[i] = newMaterial;
    }

    void Mesh::setDiffuse(std::shared_ptr<Texture> tex, int i) { materials[i].diffuse = tex; needsUpdate = true; }
    void Mesh::setSpecular(std::shared_ptr<Texture> tex, int i) { materials[i].specular = tex; needsUpdate = true; }
    void Mesh::setNormal(std::shared_ptr<Texture> tex, int i) { materials[i].normal = tex; needsUpdate = true; }
    void Mesh::setHeight(std::shared_ptr<Texture> tex, int i) { materials[i].height = tex; needsUpdate = true; }

    std::weak_ptr<Material> Mesh::GetMaterial(int i) const {
        return std::make_shared<Material>(materials[i]);
    }
    std::weak_ptr<std::vector<Material>> Mesh::GetMaterials() const { return std::make_shared<std::vector<Material>>(materials); }

    void Mesh::updateVertices(const std::vector<Vertex>& newVertices)
    {
        vertices = newVertices;

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    }

    void Mesh::render()
    {
        glBindVertexArray(VAO);

        bindMaterial();

        updateMaterial();

        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        unbindMaterial();
    }
}