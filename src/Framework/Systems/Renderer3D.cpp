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
        renderTransparentEntities(transparentEntities);
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

    void Renderer3D::renderTransparentEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& transparentEntities)
    {
        std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> opaqueMeshes;
        std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> transparentMeshes;

        // Clasificar mallas por opacidad
        for (auto& entity : transparentEntities)
        {
            auto transform = entity->GetComponent<Transform>();
            auto model = entity->GetComponent<Model>();

            for (auto& mesh : model->meshes)
            {
                if (mesh->materials[0].hasAlpha())
                {
                    transparentMeshes.emplace_back(entity, mesh);
                }
                else
                {
                    opaqueMeshes.emplace_back(entity, mesh);
                }
            }
        }

        // Renderizar mallas opacas primero
        for (auto& [entity, mesh] : opaqueMeshes)
        {
            auto transform = entity->GetComponent<Transform>();
            transform->updateMatrix();

            mesh->shader->use();
            mesh->shader->setUniform("model", transform->getModelMatrix());
            if (camera)
            {
                mesh->shader->setUniform("view", camera->getViewMatrix());
                mesh->shader->setUniform("projection", camera->getProjectionMatrix());
                mesh->shader->setUniform("viewPos", camera->getPosition());
            }

            mesh->render();
            mesh->shader->disable();
        }

        // Ordenar mallas transparentes por distancia
        std::sort(transparentMeshes.begin(), transparentMeshes.end(),
            [this](const std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>& a, const std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>& b)
            {
                auto positionA = a.first.lock()->GetComponent<Transform>()->getPosition();
                auto positionB = b.first.lock()->GetComponent<Transform>()->getPosition();

                float distanceA = glm::length(camera->getPosition() - positionA);
                float distanceB = glm::length(camera->getPosition() - positionB);

                return distanceA > distanceB;
            });

        // Renderizar mallas transparentes
        for (auto& [entity, mesh] : transparentMeshes)
        {
            auto transform = entity->GetComponent<Transform>();
            transform->updateMatrix();

            mesh->shader->use();
            mesh->shader->setUniform("model", transform->getModelMatrix());
            if (camera)
            {
                mesh->shader->setUniform("view", camera->getViewMatrix());
                mesh->shader->setUniform("projection", camera->getProjectionMatrix());
                mesh->shader->setUniform("viewPos", camera->getPosition());
            }

            mesh->render();
            mesh->shader->disable();
        }
    }
}