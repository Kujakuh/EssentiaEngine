#ifndef CAMERA_ORTHO_H
#define CAMERA_ORTHO_H

#include <Core/Systems/CameraBase.hpp>

namespace Essentia
{
    class CameraOrtho : public CameraBase
    {
        private:
            float left;          // Límite izquierdo
            float right;         // Límite derecho
            float bottom;        // Límite inferior
            float top;           // Límite superior
            float nearPlane;     // Plano cercano
            float farPlane;      // Plano lejano

        public:
            CameraOrtho(const std::string& name, Scene* bindScene,
                float left, float right, float bottom, float top, float nearPlane, float farPlane);

            glm::mat4 getViewMatrix() const override;
            glm::mat4 getProjectionMatrix() const;

            void setLeft(float newLeft);
            void setRight(float newRight);
            void setBottom(float newBottom);
            void setTop(float newTop);
            void setNearPlane(float newNearPlane);
            void setFarPlane(float newFarPlane);
    };
}

#endif // CAMERA_ORTHO_H