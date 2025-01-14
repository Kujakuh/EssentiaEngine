#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/ECS/IComponent.hpp>
#include <Shared/enums.hpp>

namespace Essentia
{
    struct Transform : IComponent
    {
        private:
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
            glm::mat4 modelMatrix;
            bool needsUpdate;

        public:
            Transform();
            Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl);

            void setPosition(const glm::vec3& pos);
            void setRotation(const glm::quat& rot);
            void setScale(const glm::vec3& scl);

            glm::vec3& setPosition();
            glm::quat& setRotation();
            glm::vec3& setScale();

            glm::vec3 getPosition() const;
            glm::quat getRotation() const;
            glm::vec3 getScale() const;
            glm::mat4 getModelMatrix() const;

            void updateMatrix();
            void rotate(const glm::vec3& eulerAngles, bool defautAxis = true,
                const glm::vec3& customAxisX = glm::vec3(1.0f, 0.0f, 0.0f),
                const glm::vec3& customAxisY = glm::vec3(0.0f, 1.0f, 0.0f),
                const glm::vec3& customAxisZ = glm::vec3(0.0f, 0.0f, 1.0f));
            void rotate(const glm::vec3& eulerAngles, const glm::vec3& centerPoint);

            bool areAligned(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.001f);

            friend class Renderer3D;
    };
}

#endif // !TRANSFORM_H
