#include <Framework/Camera/CameraBase.hpp>

namespace Essentia
{
    CameraBase::CameraBase(const std::string& name, Scene* bindScene) :
        scene(bindScene),
        entity(bindScene->CreateEntity(name))
    {
        if (!scene) throw std::runtime_error("CameraBase requires a valid Scene instance.");

        transform = entity->GetComponent<Transform>();
        if (!transform)
            throw std::runtime_error("CameraBase requires a valid Transform component.");

        front = glm::vec3(0.0f, 0.0f, -1.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }

    void CameraBase::updateCameraVectors()
    {
        if (transform)
        {
            front = glm::normalize(transform->getRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
            right = glm::normalize(glm::cross(front, worldUp));
            up = glm::normalize(glm::cross(right, front));
        }
    }

    glm::vec3 CameraBase::getFront() const { return front; }

    glm::vec3 CameraBase::getPosition() const
    {
        if (transform) return transform->getPosition();
        throw std::runtime_error("Transform is not available for this Camera");
    }
}
