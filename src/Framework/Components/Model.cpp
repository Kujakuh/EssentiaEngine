#include <Framework/Components/Model.hpp>

namespace Essentia
{
    Model::Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes): meshes(initialMeshes) {}

    void Model::addMesh(const std::shared_ptr<Mesh>& mesh) { meshes.push_back(mesh); }

    size_t Model::getMeshCount() const { return meshes.size(); }
    const std::shared_ptr<Mesh>& Model::getMesh(size_t index) const
    {
        if (index >= meshes.size())
            throw std::out_of_range("Mesh index out of range");

        return meshes[index];
    }

    std::vector<std::string> Model::getTexturePaths() const
    {
        std::vector<std::string> paths;

        for (const auto& mesh : meshes)
        {
            for (const auto& texturePair : mesh->textures)
            {
                if (texturePair.second) 
                    paths.push_back(TextureManager::getTexturePath(texturePair.second));
            }
        }

        return paths;
    }
}
