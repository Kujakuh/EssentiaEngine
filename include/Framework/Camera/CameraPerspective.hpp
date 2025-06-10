#ifndef CAMERA_PERSPECTIVE_H
#define CAMERA_PERSPECTIVE_H

#include <Framework/Camera/CameraBase.hpp>

namespace Essentia
{
    class CameraPerspective : public CameraBase
    {
        private:
            float fov;
            float aspectRatio;
            float nearPlane;
            float farPlane;

            double lastX = 0.0;
            double lastY = 0.0;
            bool firstMouse = true;

        public:
            CameraPerspective() = default;
            CameraPerspective(const std::string& name,
                               float fov, float aspectRatio, float nearPlane, float farPlane);

            glm::mat4 getViewMatrix() const override;
            glm::mat4 getProjectionMatrix() const override;

            void setFov(float newFov);
            void setAspectRatio(float newAspectRatio);
            void setNearPlane(float newNearPlane);
            void setFarPlane(float newFarPlane);

            void processMouseMovement(double xpos, double ypos, float deltaTime = 1.0f);
    };
}

#endif // !CAMERA_PERSPECTIVE_H