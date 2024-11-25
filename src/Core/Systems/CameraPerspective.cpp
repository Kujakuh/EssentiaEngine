#include <Core/Systems/CameraPerspective.hpp>

namespace Essentia
{
    CameraPerspective::CameraPerspective(const std::string& name, Scene* bindScene,
        float fov, float aspectRatio, float nearPlane, float farPlane) :
        CameraBase(name, bindScene),
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
}
