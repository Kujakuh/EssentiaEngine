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
            Renderer2D(CameraBase* camera) { setCamera(camera); }
            CameraBase* camera;

            SORT_ORDER sortOrder = SORT_ORDER::Z_ORDER;

            void setCamera(CameraBase* camera) { this->camera = camera; }
            void setSortOrder(SORT_ORDER newSortOrder) { sortOrder = newSortOrder; }

            void Update(EntityManager& entityManager) override
            {
                auto entities = entityManager.GetEntitiesWith<Transform, Sprite>();

                std::sort(entities.begin(), entities.end(), [this](Essentia::WeakptrWrapper<Entity> a, Essentia::WeakptrWrapper<Entity> b)
                    {
                        auto positionA = a->GetComponent<Transform>()->getPosition();
                        auto positionB = b->GetComponent<Transform>()->getPosition();

                        if (sortOrder == SORT_ORDER::Z_ORDER) return positionA.z < positionB.z;  // Ordenar por Z
                        else return positionA.y < positionB.y;  // Ordenar por Y
                    });

                for (auto& entity : entities)
                {
                    auto transform = entity->GetComponent<Transform>();
                    auto sprite = entity->GetComponent<Sprite>();

                    transform->updateMatrix();
                    sprite->mesh->initShader();

                    sprite->mesh->shader.setUniform("model", transform->getModelMatrix());

                    if (camera)
                    {
                        sprite->mesh->shader.setUniform("view", camera->getViewMatrix());
                        sprite->mesh->shader.setUniform("projection", camera->getProjectionMatrix());
                    }

                    sprite->mesh->render();
                    sprite->mesh->disableShader();
                }
            }

            ~Renderer2D() override = default;
    };
}

#endif // !RENDERER2D_H
