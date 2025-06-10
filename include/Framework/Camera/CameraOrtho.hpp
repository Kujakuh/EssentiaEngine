#ifndef CAMERA_ORTHO_H
#define CAMERA_ORTHO_H

#include <Framework/Camera/CameraBase.hpp>

namespace Essentia
{
    class CameraOrtho : public CameraBase
    {
        private:
            float left;
            float right;
            float bottom;
            float top;
            float nearPlane;
            float farPlane;

        public:
            CameraOrtho(const std::string& name,
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

#endif // !CAMERA_ORTHO_H