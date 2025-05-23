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

			bool _hasBoneWeights;

            std::vector<Material> materials;
            std::shared_ptr<Shader> shader;

            Mesh() = default;
            Mesh(std::shared_ptr<Shader> sh, std::vector<Vertex> vert, std::vector<GLuint> ind, std::vector<Material> mat, bool hasBoneWeights = false)
                : vertices(vert), indices(ind), materials(mat), shader(sh), _hasBoneWeights(hasBoneWeights)
            {
                if (_hasBoneWeights) {
                    for (auto& v : vertices)
                        v.NormalizeWeights();
                }

                setupMesh();
                needsUpdate = true;
            }

            void render();
            void initShader() const { shader->use(); }
            void disableShader() const { shader->disable(); }
            void updateVertices(const std::vector<Vertex>& newVertices);

            void SetMaterial(const Material& newMaterial, int i);

            void setDiffuse(std::shared_ptr<Texture> tex, int i = 0);
            void setSpecular(std::shared_ptr<Texture> tex, int i = 0);
            void setNormal(std::shared_ptr<Texture> tex, int i = 0);
            void setHeight(std::shared_ptr<Texture> tex, int i = 0);
            void setAlpha(std::shared_ptr<Texture> tex, int i = 0);
            void setAlbedo(std::shared_ptr<Texture> tex, int i = 0);
            void setRoughness(std::shared_ptr<Texture> tex, int i = 0);
            void setMetallic(std::shared_ptr<Texture> tex, int i = 0);
            void setAo(std::shared_ptr<Texture> tex, int i = 0);
            void setEmissive(std::shared_ptr<Texture> tex, int i = 0);

        private:
            unsigned int VBO, EBO;
            bool needsUpdate;

            void updateMaterial(int i = 0);
            void bindMaterial(int i = 0);
            void unbindMaterial();

            void setupMesh();

            friend class Model;
            friend class Renderer3D;
    };
}

#endif // !MESH_H