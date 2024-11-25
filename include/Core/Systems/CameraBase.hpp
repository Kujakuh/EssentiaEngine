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
#include <Core/Components/Transform.hpp>
#include <Core/Memory/WeakptrWrapper.hpp>

namespace Essentia
{
    class CameraBase
    {
        protected:
            Scene* scene;

            glm::vec3 front;        // Dirección hacia donde apunta la cámara
            glm::vec3 worldUp;      // Vector Up global
            glm::vec3 right;        // Vector de la derecha (calculado)
            glm::vec3 up;           // Vector Up relativo a la cámara (calculado)


            void updateCameraVectors();

        public:
            CameraBase(const std::string& name, Scene* bindScene);

            WeakptrWrapper<Entity> entity;
            Transform* transform;

            virtual glm::mat4 getViewMatrix() const = 0; // Cada tipo de cámara tendrá su implementación

            glm::vec3 getFront() const;
            glm::vec3 getPosition() const;
    };
}

#endif // CAMERA_BASE_H