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

namespace Essentia
{
    class Mesh
    {
        public:
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
            GLuint VAO;
            Shader& shader;

            Mesh(Shader& sh, std::vector<Vertex> vert, std::vector<GLuint> ind, std::unordered_map<std::string, std::shared_ptr<Texture>> tex)
                : vertices(vert), indices(ind), textures(tex), shader(sh) { setupMesh(); }

            void render();
            void initShader() const { shader.use(); }
            void disableShader() const { shader.disable(); }
            void updateVertices(const std::vector<Vertex>& newVertices);

        private:
            unsigned int VBO, EBO;

            void bindTextures();
            void unbindTextures();
            void setupMesh();
    };
}

#endif // !MESH_H