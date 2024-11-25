#include <Framework/Camera/CameraOrtho.hpp>

namespace Essentia
{
    CameraOrtho::CameraOrtho(const std::string& name, Scene* bindScene,
        float left, float right, float bottom, float top, float nearPlane, float farPlane) :
        CameraBase(name, bindScene),
        left(left), right(right), bottom(bottom), top(top), nearPlane(nearPlane), farPlane(farPlane)
    {
    }

    glm::mat4 CameraOrtho::getViewMatrix() const
    {
        // Matriz de vista basada en la posición y rotación de la cámara
        if (transform) return glm::lookAt(transform->getPosition(), transform->getPosition() + front, up);
        throw std::runtime_error("Transform is not available for this Camera");
    }

    glm::mat4 CameraOrtho::getProjectionMatrix() const
    {
        return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    }

    void CameraOrtho::setLeft(float newLeft) { left = newLeft; }
    void CameraOrtho::setRight(float newRight) { right = newRight; }
    void CameraOrtho::setBottom(float newBottom) { bottom = newBottom; }
    void CameraOrtho::setTop(float newTop) { top = newTop; }
    void CameraOrtho::setNearPlane(float newNearPlane) { nearPlane = newNearPlane; }
    void CameraOrtho::setFarPlane(float newFarPlane) { farPlane = newFarPlane; }
}
