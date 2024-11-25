#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#define GLM_ENABLE_EXPERIMENTAL

#include <string>
#include <memory>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Core/ECS/Entity.hpp>
#include <Core/ECS/Scene.hpp>
#include <Core/ECS/EntityManager.hpp>
#include <Framework/Components/Transform.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>

namespace Essentia
{
    class CameraBase
    {
        protected:
            Scene* scene;

            glm::vec3 front;
            glm::vec3 worldUp;
            glm::vec3 right;
            glm::vec3 up;

            void updateCameraVectors();

        public:
            CameraBase(const std::string& name, Scene* bindScene);

            WeakptrWrapper<Entity> entity;
            Transform* transform;

            float sensitivity = 0.01;

            void setFront(glm::vec3 _front);
            virtual glm::mat4 getViewMatrix() const = 0;

            glm::vec3 getFront() const;
            glm::vec3 getRight() const;
            glm::vec3 getUp() const;
            glm::vec3 getWUp() const;
            glm::vec3 getPosition() const;
    };
}

#endif // CAMERA_BASE_H