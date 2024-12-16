#include <Framework/Systems/Renderer3D.hpp>
#include <Debug/glmDebug.hpp>

namespace Essentia
{
    Renderer3D::Renderer3D(CameraBase* camera) 
    { 
        setCamera(camera); 
        std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> opaqueMeshes;
        std::vector<std::pair<WeakptrWrapper<Entity>, std::shared_ptr<Mesh>>> transparentMeshes;
        std::vector<WeakptrWrapper<Entity>> opaqueEntities;
        std::vector<WeakptrWrapper<Entity>> transparentEntities;

        std::vector<float> meshDistances;
    }

    void Renderer3D::setCamera(CameraBase* camera) { this->camera = camera; }

    void Renderer3D::Update(EntityManager& entityManager)
    {
        auto entitiesWithLights = entityManager.GetEntitiesWith<LightSource>();
        auto entitiesWithModels = entityManager.GetEntitiesWith<Transform, Model>();

        for (auto& entity : entitiesWithModels)
        {
            auto model = entity->GetComponent<Model>();

            if (model && model->hasAlpha())
                transparentEntities.push_back(entity);
            else
                opaqueEntities.push_back(entity);
        }

        renderEntities(opaqueEntities, entitiesWithLights);
        renderTransparentEntities(transparentEntities, entitiesWithLights);

        opaqueEntities.clear();
        transparentEntities.clear();
    }

    void Renderer3D::renderEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& entities, const std::vector<WeakptrWrapper<Entity>>& lights)
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

                mesh->shader->setUniform("lightsNum", static_cast<int>(lights.size()));
                for (int i = 0; i < lights.size(); ++i)
                {
                    auto ls = lights[i].weakPtr_.lock()->GetComponent<LightSource>();
                    auto ts = lights[i].weakPtr_.lock()->GetComponent<Transform>();

                    const std::string lightBase = "lights[" + std::to_string(i) + "]";
                    mesh->shader->setUniform(lightBase + ".position", ts->getPosition());
                    mesh->shader->setUniform(lightBase + ".direction", ls->GetDirection());
                    if (ls->needsUpdate || mesh->needsUpdate)
                    {
                        mesh->shader->setUniform(lightBase + ".type", static_cast<int>(ls->GetType()));
                        mesh->shader->setUniform(lightBase + ".color", ls->GetColor());
                        mesh->shader->setUniform(lightBase + ".constant", ls->GetConstant());
                        mesh->shader->setUniform(lightBase + ".linear", ls->GetLinear());
                        mesh->shader->setUniform(lightBase + ".quadratic", ls->GetQuadratic());
                        mesh->shader->setUniform(lightBase + ".intensity", ls->GetIntensity());
                        mesh->shader->setUniform(lightBase + ".innerCutoff", ls->GetInnerCutOff());
                        mesh->shader->setUniform(lightBase + ".outerCutoff", ls->GetOuterCutOff());
                        ls->needsUpdate = false;
                    }
                }
                mesh->render();
            }
            model->getShader()->disable();
        }
    }

    void Renderer3D::renderTransparentEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& transparentEntities, const std::vector<WeakptrWrapper<Entity>>& lights)
    {
        for (auto& entity : transparentEntities)
        {
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

            mesh->shader->setUniform("lightsNum", static_cast<int>(lights.size()));
            for (int i = 0; i < lights.size(); ++i)
            {
                auto ls = lights[i].weakPtr_.lock()->GetComponent<LightSource>();
                auto ts = lights[i].weakPtr_.lock()->GetComponent<Transform>();

                const std::string lightBase = "lights[" + std::to_string(i) + "]";
                mesh->shader->setUniform(lightBase + ".position", ts->getPosition());
                mesh->shader->setUniform(lightBase + ".direction", ls->GetDirection());
                if (ls->needsUpdate || mesh->needsUpdate)
                {
                    mesh->shader->setUniform(lightBase + ".type", static_cast<int>(ls->GetType()));
                    mesh->shader->setUniform(lightBase + ".color", ls->GetColor());
                    mesh->shader->setUniform(lightBase + ".constant", ls->GetConstant());
                    mesh->shader->setUniform(lightBase + ".linear", ls->GetLinear());
                    mesh->shader->setUniform(lightBase + ".quadratic", ls->GetQuadratic());
                    mesh->shader->setUniform(lightBase + ".intensity", ls->GetIntensity());
                    mesh->shader->setUniform(lightBase + ".innerCutoff", ls->GetInnerCutOff());
                    mesh->shader->setUniform(lightBase + ".outerCutoff", ls->GetOuterCutOff());
                    ls->needsUpdate = false;
                }
            }

            mesh->render();
            mesh->shader->disable();
        }

        meshDistances.resize(transparentMeshes.size());

        for (size_t i = 0; i < transparentMeshes.size(); ++i)
        {
            auto entity = transparentMeshes[i].first.weakPtr_.lock();
            if (entity)
            {
                // If transform changed
                float newDistance = glm::length(camera->getPosition() - entity->GetComponent<Transform>()->getPosition());

                if (meshDistances[i] != newDistance)
                {
                    meshDistances[i] = newDistance;

                    auto& pair = transparentMeshes[i];
                    float distance = meshDistances[i];
                    size_t j = i;
                    while (j > 0 && meshDistances[j - 1] < distance)
                    {
                        transparentMeshes[j] = transparentMeshes[j - 1];
                        meshDistances[j] = meshDistances[j - 1];
                        --j;
                    }
                    transparentMeshes[j] = pair;
                    meshDistances[j] = distance;
                }
            }
        }

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

            mesh->shader->setUniform("lightsNum", static_cast<int>(lights.size()));
            for (int i = 0; i < lights.size(); ++i)
            {
                auto ls = lights[i].weakPtr_.lock()->GetComponent<LightSource>();
                auto ts = lights[i].weakPtr_.lock()->GetComponent<Transform>();

                const std::string lightBase = "lights[" + std::to_string(i) + "]";
                mesh->shader->setUniform(lightBase + ".position", ts->getPosition());
                mesh->shader->setUniform(lightBase + ".direction", ls->GetDirection());
                if (ls->needsUpdate || mesh->needsUpdate)
                {
                    mesh->shader->setUniform(lightBase + ".type", static_cast<int>(ls->GetType()));
                    mesh->shader->setUniform(lightBase + ".color", ls->GetColor());
                    mesh->shader->setUniform(lightBase + ".constant", ls->GetConstant());
                    mesh->shader->setUniform(lightBase + ".linear", ls->GetLinear());
                    mesh->shader->setUniform(lightBase + ".quadratic", ls->GetQuadratic());
                    mesh->shader->setUniform(lightBase + ".intensity", ls->GetIntensity());
                    mesh->shader->setUniform(lightBase + ".innerCutoff", ls->GetInnerCutOff());
                    mesh->shader->setUniform(lightBase + ".outerCutoff", ls->GetOuterCutOff());
                    ls->needsUpdate = false;
                }
            }

            mesh->render();
            mesh->shader->disable();
        }

        opaqueMeshes.clear();
        transparentMeshes.clear();
    }
}
