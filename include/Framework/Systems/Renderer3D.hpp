#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <vector>
#include <algorithm>
#include <Core/ECS/EntityManager.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>
#include <Framework/Camera/CameraBase.hpp>
#include <Framework/Components/Model/Model.hpp>
#include <Framework/Components/LightSource.hpp>

namespace Essentia
{
    class Renderer3D : public ISystem
    {
        public:
            Renderer3D() = default;
            Renderer3D(CameraBase* camera);
            void setCamera(CameraBase* camera);

            void Update(EntityManager& entityManager);

        private:
            CameraBase* camera = nullptr;

            std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> opaqueMeshes;
            std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> transparentMeshes;
            std::vector<WeakptrWrapper<Entity>> opaqueEntities;
            std::vector<WeakptrWrapper<Entity>> transparentEntities;

            std::vector<float> meshDistances;

            //void uploadLightsToShader(std::shared_ptr<Shader> shader, const std::vector<WeakptrWrapper<Entity>>& lights);
            void renderEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& entities);
            void renderTransparentEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& transparentEntities);
    };
}

#endif // RENDERER3D_H