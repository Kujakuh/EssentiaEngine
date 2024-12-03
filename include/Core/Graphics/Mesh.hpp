#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include <Core/Graphics/Vertex.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/Shader.hpp>
#include <Core/Graphics/Material.hpp>

namespace Essentia
{
    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            GLuint VAO;

            Material material;
            std::shared_ptr<Shader> shader;

            Mesh() = default;
            Mesh(std::shared_ptr<Shader> sh, std::vector<Vertex> vert, std::vector<GLuint> ind, Material mat)
                : vertices(vert), indices(ind), material(mat), shader(sh) { setupMesh(); needsUpdate = true; }

            void render();
            void initShader() const { shader->use(); }
            void disableShader() const { shader->disable(); }
            void updateVertices(const std::vector<Vertex>& newVertices);

            void SetMaterial(const Material& newMaterial);
            std::weak_ptr<Material> GetMaterial() const;

            void setDiffuse(std::shared_ptr<Texture> tex);
            void setSpecular(std::shared_ptr<Texture> tex);
            void setNormal(std::shared_ptr<Texture> tex);
            void setHeight(std::shared_ptr<Texture> tex);

        private:
            unsigned int VBO, EBO;
            bool needsUpdate;

            void updateMaterial();
            void bindMaterial();
            void unbindMaterial();

            void setupMesh();
    };
}

#endif // !MESH_H