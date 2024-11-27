#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <Core/ECS/ISystem.hpp>
#include <Core/ECS/Entity.hpp>
#include <Core/ECS/EntityManager.hpp>

namespace Essentia
{
    class Renderer2D : public ISystem
    {
        public:
            Renderer2D() = default;

            void Update(EntityManager& entityManager) override
            {
                for (auto& entity : entityManager.GetEntitiesWith<Transform, Sprite>())
                {
                    entity->GetComponent<Transform>()->updateMatrix();

                    entity->GetComponent<Sprite>()->mesh->initShader();
                    entity->GetComponent<Sprite>()->mesh->shader.setUniform("model", entity->GetComponent<Transform>()->getModelMatrix());

                    entity->GetComponent<Sprite>()->mesh->render();
                    entity->GetComponent<Sprite>()->mesh->disableShader();
                }
            }

            ~Renderer2D() override = default;
    };
}

#endif // !RENDERER2D_H
