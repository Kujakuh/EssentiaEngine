#include <Framework/Systems/Renderer3D.hpp>

namespace Essentia
{
    Renderer3D::Renderer3D(CameraBase* camera) { setCamera(camera); }

    void Renderer3D::setCamera(CameraBase* camera) { this->camera = camera; }

    void Renderer3D::Update(EntityManager& entityManager)
    {
        auto entities = entityManager.GetEntitiesWith<Transform, Model>();

        std::vector<WeakptrWrapper<Entity>> opaqueEntities;
        std::vector<WeakptrWrapper<Entity>> transparentEntities;

        for (auto& entity : entities)
        {
            auto model = entity->GetComponent<Model>();

            if (model && model->hasAlpha()) transparentEntities.push_back(entity);
            else opaqueEntities.push_back(entity);
        }

        renderEntities(opaqueEntities);

        std::sort(transparentEntities.begin(), transparentEntities.end(), [this](Essentia::WeakptrWrapper<Entity> a, Essentia::WeakptrWrapper<Entity> b)
        {
            auto positionA = a->GetComponent<Transform>()->getPosition();
            auto positionB = b->GetComponent<Transform>()->getPosition();

            float distanceA = glm::length(camera->getPosition() - positionA);
            float distanceB = glm::length(camera->getPosition() - positionB);

            return distanceA > distanceB;
        });

        renderEntities(transparentEntities);
    }

    void Renderer3D::renderEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& entities)
    {
        for (auto& entity : entities)
        {
            auto transform = entity->GetComponent<Transform>();
            auto model = entity->GetComponent<Model>();

            transform->updateMatrix();

            model->getShader()->use();
            for (auto& mesh : model->meshes)
            {
                mesh->shader->setUniform("model", transform->getModelMatrix());
                if (camera)
                {
                    mesh->shader->setUniform("view", camera->getViewMatrix());
                    mesh->shader->setUniform("projection", camera->getProjectionMatrix());
                    mesh->shader->setUniform("viewPos", camera->getPosition());
                }

                mesh->render();
            }
            model->getShader()->disable();
        }
    }
}