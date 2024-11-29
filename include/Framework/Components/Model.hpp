#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/TextureManager.hpp>

namespace Essentia
{
    struct Model : public IComponent
    {
        public:
            std::vector<std::shared_ptr<Mesh>> meshes;

            Model() = default;
            Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes);

            void addMesh(const std::shared_ptr<Mesh>& mesh);

            size_t getMeshCount() const;
            const std::shared_ptr<Mesh>& getMesh(size_t index) const;
            std::vector<std::string> getTexturePaths() const;
    };
}

#endif // !MODEL_H