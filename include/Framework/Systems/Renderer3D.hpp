#ifndef RENDERER3D_H
#define RENDERER3D_H

#include <vector>
#include <algorithm>
#include <Core/ECS/EntityManager.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>
#include <Framework/Camera/CameraBase.hpp>
#include <Framework/Components/Model/Model.hpp>

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
            void renderEntities(std::vector<WeakptrWrapper<Entity>>& entities);
            void renderTransparentEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& transparentEntities);
    };
}
#endif // RENDERER3D_H
