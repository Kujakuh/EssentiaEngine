#include <Framework/Camera/CameraPerspective.hpp>
#include <Framework/Time/Time.hpp>

namespace Essentia
{
    CameraPerspective::CameraPerspective(const std::string& name,
        float fov, float aspectRatio, float nearPlane, float farPlane) :
        CameraBase(name),
        fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane) {}

    glm::mat4 CameraPerspective::getViewMatrix() const
    {
        if (transform) return glm::lookAt(transform->getPosition(), transform->getPosition() + front, up);
        throw std::runtime_error("Transform is not available for this Camera");
    }

    glm::mat4 CameraPerspective::getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void CameraPerspective::setFov(float newFov) { fov = newFov; }
    void CameraPerspective::setAspectRatio(float newAspectRatio) { aspectRatio = newAspectRatio; }
    void CameraPerspective::setNearPlane(float newNearPlane) { nearPlane = newNearPlane; }
    void CameraPerspective::setFarPlane(float newFarPlane) { farPlane = newFarPlane; }

    void CameraPerspective::processMouseMovement(double xpos, double ypos, float speed)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
            return;
        }

        float deltaX = static_cast<float>(xpos - lastX);
        float deltaY = static_cast<float>(lastY - ypos);

        lastX = xpos;
        lastY = ypos;

        deltaX *= sensitivity * speed * Time::deltaTime();
        deltaY *= sensitivity * speed * Time::deltaTime();

        transform->rotate(glm::vec3(deltaY, deltaX, 0.0f), false, getRight(), glm::vec3(0, 1, 0), getFront());

        if (transform->areAligned(glm::normalize(transform->getRotation() * glm::vec3(0.0f, 0.0f, -1.0f)),  worldUp, 0.0005) ||
            transform->areAligned(glm::normalize(transform->getRotation() * glm::vec3(0.0f, 0.0f, -1.0f)), -worldUp, 0.0005))
                transform->rotate(glm::vec3(-deltaY, 0.0f, 0.0f), false, getRight(), glm::vec3(0, 1, 0), getFront());

        transform->updateMatrix();
        updateCameraVectors();
    }
}
