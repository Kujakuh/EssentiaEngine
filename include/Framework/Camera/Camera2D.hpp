#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <Framework/Camera/CameraPerspective.hpp>

namespace Essentia
{
    class Camera2D : public CameraPerspective
    {
        public:
            Camera2D(const std::string& name,
                float fov, float aspectRatio, float nearPlane, float farPlane);

            glm::mat4 getViewMatrix() const override;
    };
}

#endif // !CAMERA_2D_H