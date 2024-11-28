#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/TextureManager.hpp>

namespace Essentia
{
    class Model : public IComponent
    {
        public:
            std::vector<std::shared_ptr<Mesh>> meshes;

            Model() = default;
            Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes) : meshes(initialMeshes) {}

            void addMesh(const std::shared_ptr<Mesh>& mesh) { meshes.push_back(mesh); }

            size_t getMeshCount() const { return meshes.size(); }
            const std::shared_ptr<Mesh>& getMesh(size_t index) const
            {
                if (index >= meshes.size()) throw std::out_of_range("Mesh index out of range");
                return meshes[index];
            }
            std::vector<std::string> getTexturePaths() const
            {
                std::vector<std::string> paths;

                for (const auto& mesh : meshes)
                    for (const auto& texturePair : mesh->textures)
                        if (texturePair.second) paths.push_back(TextureManager::getTexturePath(texturePair.second));

                return paths;
            }
    };
}

#endif // MODEL_H