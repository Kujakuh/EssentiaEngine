#include <Framework/Systems/Renderer2D.hpp>

namespace Essentia
{
    Renderer2D::Renderer2D(CameraBase* camera) { setCamera(camera); }

    void Renderer2D::setCamera(CameraBase* camera) { this->camera = camera; }

    void Renderer2D::setSortOrder(SORT_ORDER newSortOrder) { sortOrder = newSortOrder; }

    void Renderer2D::Update(EntityManager& entityManager)
    {
        auto entities = entityManager.GetEntitiesWith<Transform, Sprite>();

        std::sort(entities.begin(), entities.end(), [this](Essentia::WeakptrWrapper<Entity> a, Essentia::WeakptrWrapper<Entity> b)
            {
                auto positionA = a->GetComponent<Transform>()->getPosition();
                auto positionB = b->GetComponent<Transform>()->getPosition();

                return (sortOrder == SORT_ORDER::Z_ORDER) ? positionA.z < positionB.z : positionA.y < positionB.y;
            });

        for (auto& entity : entities)
        {
            auto transform = entity->GetComponent<Transform>();
            auto sprite = entity->GetComponent<Sprite>();

            transform->updateMatrix();
            sprite->mesh->initShader();

            sprite->mesh->shader->setUniform("model", transform->getModelMatrix());

            if (camera)
            {
                sprite->mesh->shader->setUniform("view", camera->getViewMatrix());
                sprite->mesh->shader->setUniform("projection", camera->getProjectionMatrix());
            }

            sprite->mesh->render();
            sprite->mesh->disableShader();
        }
    }

    Renderer2D::~Renderer2D() = default;
}