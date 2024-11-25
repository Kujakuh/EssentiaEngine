#ifndef CAMERA_PERSPECTIVE_H
#define CAMERA_PERSPECTIVE_H

#include <Framework/Camera/CameraBase.hpp>

namespace Essentia
{
    class CameraPerspective : public CameraBase
    {
        private:
            float fov;           // Campo de visión
            float aspectRatio;   // Relación de aspecto
            float nearPlane;     // Plano cercano
            float farPlane;      // Plano lejano

            double lastX = 0.0;
            double lastY = 0.0;
            bool firstMouse = true; // Manejo del primer movimiento del ratón

        public:
            CameraPerspective(const std::string& name, Scene* bindScene,
                               float fov, float aspectRatio, float nearPlane, float farPlane);

            glm::mat4 getViewMatrix() const override;
            glm::mat4 getProjectionMatrix() const;

            void setFov(float newFov);
            void setAspectRatio(float newAspectRatio);
            void setNearPlane(float newNearPlane);
            void setFarPlane(float newFarPlane);

            void processMouseMovement(double xpos, double ypos);
    };
}

#endif // CAMERA_PERSPECTIVE_H