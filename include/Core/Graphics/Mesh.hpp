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

namespace Essentia
{
    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            GLuint VAO;
            std::shared_ptr<Shader> shader;

            Mesh() = default;
            Mesh(std::shared_ptr<Shader> sh, std::vector<Vertex> vert, std::vector<GLuint> ind, std::unordered_map<std::string, std::shared_ptr<Texture>> tex)
                : vertices(vert), indices(ind), textures(tex), shader(sh) { setupMesh(); needsUpdate = true; }

            void render();
            void initShader() const { shader->use(); }
            void disableShader() const { shader->disable(); }
            void updateVertices(const std::vector<Vertex>& newVertices);
            void SetTexture(const std::string& name, std::shared_ptr<Texture> texture);
            std::shared_ptr<Texture> GetTexture(const std::string& name) const;
            const std::unordered_map<std::string, std::shared_ptr<Texture>>& GetAllTextures();
            void SetAllTextures(std::unordered_map<std::string, std::shared_ptr<Texture>> newTextures);

        private:
            unsigned int VBO, EBO;
            std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
            bool needsUpdate;

            void bindTextures();
            void updateTextures();
            void unbindTextures();
            void setupMesh();
    };
}

#endif // !MESH_H