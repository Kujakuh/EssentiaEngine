#include <Framework/Camera/Camera2D.hpp>

namespace Essentia
{
    Camera2D::Camera2D(const std::string& name, Scene* bindScene,
        float fov, float aspectRatio, float nearPlane, float farPlane) :
        CameraPerspective(name, bindScene, fov, aspectRatio, nearPlane, farPlane)
    {
        if (transform) transform->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    }

    glm::mat4 Camera2D::getViewMatrix() const
    {
        if (transform)
        {
            glm::vec3 position = transform->getPosition();
            position.z = 10.0f; // Fijar Z
            return glm::lookAt(position, glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        throw std::runtime_error("Transform is not available for this Camera");
    }
}
