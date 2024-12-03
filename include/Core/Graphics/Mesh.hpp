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
#include <Debug/openglDebug.hpp>
#include <Core/Graphics/Material.hpp>

namespace Essentia
{
    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            GLuint VAO;
            std::shared_ptr<Shader> shader;
            Material material;

            Mesh() = default;
            Mesh(std::shared_ptr<Shader> sh, std::vector<Vertex> vert, std::vector<GLuint> ind, Material mat)
                : vertices(vert), indices(ind), material(mat), shader(sh) { setupMesh(); needsUpdate = true; }

            void render();
            void initShader() const { shader->use(); }
            void disableShader() const { shader->disable(); }
            void updateVertices(const std::vector<Vertex>& newVertices);

            /*void SetTexture(const std::string& name, std::shared_ptr<Texture> texture);
            std::shared_ptr<Texture> GetTexture(const std::string& name) const;
            const std::unordered_map<std::string, std::shared_ptr<Texture>>& GetAllTextures();
            void SetAllTextures(std::unordered_map<std::string, std::shared_ptr<Texture>> newTextures);*/

            void SetMaterial(const Material& newMaterial);
            std::weak_ptr<Material> GetMaterial() const;

        private:
            unsigned int VBO, EBO;
            bool needsUpdate;

            /*void bindTextures();
            void updateTextures();
            void unbindTextures();*/

            void updateMaterial();
            void bindMaterial();
            void unbindMaterial();

            void setupMesh();
    };
}

#endif // !MESH_H