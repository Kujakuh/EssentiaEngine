#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <Core/ECS/ISystem.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/ECS/EntityManager.hpp>
#include <Framework/Camera/CameraBase.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>
#include <Shared/enums.hpp>

#include <vector>
#include <algorithm>

namespace Essentia
{
    class Renderer2D : public ISystem
    {
        public:
            Renderer2D() = default;
            Renderer2D(CameraBase* camera);

            void setCamera(CameraBase* camera);
            void setSortOrder(SORT_ORDER newSortOrder);

            void Update(EntityManager& entityManager) override;

            ~Renderer2D() override;

        private:
            CameraBase* camera = nullptr;
            SORT_ORDER sortOrder = SORT_ORDER::Z_ORDER;
    };
}

#endif // !RENDERER2D_H