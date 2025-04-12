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

        for (auto& entity : entitiesWithModels)
        {
            auto& shader = entity->GetComponent<Model>()->getShader();
            auto mod = entity->GetComponent<Model>();
            shader->use();
            shader->setUniform("lightsNum", static_cast<int>(entitiesWithLights.size()));
            for (int i = 0; i < entitiesWithLights.size(); ++i)
            {
                auto lightEntity = entitiesWithLights[i].weakPtr_.lock();
                if (!lightEntity) continue;

                auto ls = lightEntity->GetComponent<LightSource>();
                auto ts = lightEntity->GetComponent<Transform>();

                const std::string lightBase = "lights[" + std::to_string(i) + "]";

                if(ts->needsUpdate) shader->setUniform(lightBase + ".position", ts->getPosition());
                if(ls->type == LightType::Spot) shader->setUniform(lightBase + ".direction", ls->GetDirection());

                if (ls->needsUpdate || mod->getMesh(0)->needsUpdate)
                {
                    shader->setUniform(lightBase + ".type", static_cast<int>(ls->GetType()));
                    shader->setUniform(lightBase + ".color", ls->GetColor());
                    shader->setUniform(lightBase + ".constant", ls->GetConstant());
                    shader->setUniform(lightBase + ".linear", ls->GetLinear());
                    shader->setUniform(lightBase + ".quadratic", ls->GetQuadratic());
                    shader->setUniform(lightBase + ".intensity", ls->GetIntensity());
                    shader->setUniform(lightBase + ".innerCutoff", ls->GetInnerCutOff());
                    shader->setUniform(lightBase + ".outerCutoff", ls->GetOuterCutOff());
                    shader->setUniform(lightBase + ".diffuse", ls->GetDiffuse());
                    shader->setUniform(lightBase + ".specular", ls->GetSpecular());
                    shader->setUniform(lightBase + ".ambient", ls->GetAmbient());
                }
                ls->needsUpdate = false;
                shader->disable();
            }
        }

        renderEntities(opaqueEntities);
        renderTransparentEntities(transparentEntities);

        opaqueEntities.clear();
        transparentEntities.clear();
    }

    void Renderer3D::renderEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& entities)
    {
        for (auto& entity : entities)
        {
            auto transform = entity->GetComponent<Transform>();
            auto model = entity->GetComponent<Model>();

            transform->updateMatrix();

            auto& shader = model->getShader();
            shader->use();

            shader->setUniform("model", transform->getModelMatrix());

            if (camera)
            {
                shader->setUniform("view", camera->getViewMatrix());
                shader->setUniform("projection", camera->getProjectionMatrix());
                shader->setUniform("viewPos", camera->getPosition());
            }

            for (auto& mesh : model->meshes) 
            {
                if (mesh->_hasBoneWeights) {
                    shader->setUniform("useBones", 1);
                    for (int i = 0; i < model->skeleton.GetFinalBoneMatrices().size(); ++i)
                        model->getShader()->setUniform("finalBonesMatrices[" + std::to_string(i) + "]", model->skeleton.GetFinalBoneMatrices()[i]);
                }
                else shader->setUniform("useBones", 0);
                mesh->render();
            }
            model->getShader()->disable();
        }
    }

    void Renderer3D::renderTransparentEntities(std::vector<Essentia::WeakptrWrapper<Entity>>& transparentEntities)
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
            auto model = entity->GetComponent<Model>();
            transform->updateMatrix();

            mesh->shader->use();
            mesh->shader->setUniform("model", transform->getModelMatrix());
            if (camera)
            {
                mesh->shader->setUniform("view", camera->getViewMatrix());
                mesh->shader->setUniform("projection", camera->getProjectionMatrix());
                mesh->shader->setUniform("viewPos", camera->getPosition());
            }
            if (mesh->_hasBoneWeights) {
                mesh->shader->setUniform("useBones", 1);
                for (int i = 0; i < model->skeleton.GetFinalBoneMatrices().size(); ++i)
                    model->getShader()->setUniform("finalBonesMatrices[" + std::to_string(i) + "]", model->skeleton.GetFinalBoneMatrices()[i]);
            }
            else mesh->shader->setUniform("useBones", 0);
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
            auto model = entity->GetComponent<Model>();
            transform->updateMatrix();

            mesh->shader->use();
            mesh->shader->setUniform("model", transform->getModelMatrix());
            if (camera)
            {
                mesh->shader->setUniform("view", camera->getViewMatrix());
                mesh->shader->setUniform("projection", camera->getProjectionMatrix());
                mesh->shader->setUniform("viewPos", camera->getPosition());
            }
            if (mesh->_hasBoneWeights) {
                mesh->shader->setUniform("useBones", 1);
                for (int i = 0; i < model->skeleton.GetFinalBoneMatrices().size(); ++i)
                    model->getShader()->setUniform("finalBonesMatrices[" + std::to_string(i) + "]", model->skeleton.GetFinalBoneMatrices()[i]);
            }
            else mesh->shader->setUniform("useBones", 0);
            mesh->render();
            mesh->shader->disable();
        }

        opaqueMeshes.clear();
        transparentMeshes.clear();
    }
}
