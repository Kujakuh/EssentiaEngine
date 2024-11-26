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
            Transform() :
                position(0.0f, 0.0f, 0.0f),
                rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
                scale(1.0f, 1.0f, 1.0f),
                modelMatrix(1.0f),
                needsUpdate(false) {}

            Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl) :
                position(pos),
                rotation(rot),
                scale(scl),
                modelMatrix(1.0f),
                needsUpdate(true)
            { updateMatrix(); }

            void setPosition(const glm::vec3& pos)
            {
                position = pos;
                needsUpdate = true;
            }
            void setRotation(const glm::quat& rot)
            {
                rotation = rot;
                needsUpdate = true;
            }
            void setScale(const glm::vec3& scl)
            {
                scale = scl;
                needsUpdate = true;
            }

            glm::vec3& setPosition()
            {
                needsUpdate = true;
                return position;
            }
            glm::quat& setRotation()
            {
                needsUpdate = true;
                return rotation;
            }
            glm::vec3& setScale()
            {
                needsUpdate = true;
                return scale;
            }

            glm::vec3 getPosition() const { return position; }
            glm::quat getRotation() const { return rotation; }
            glm::vec3 getScale() const { return scale; }
            glm::mat4 getModelMatrix() const { return modelMatrix; }

            void updateMatrix()
            {
                if (needsUpdate)
                {
                    modelMatrix = glm::translate(glm::mat4(1.0f), position) *
                        glm::mat4_cast(rotation) *
                        glm::scale(glm::mat4(1.0f), scale);

                    needsUpdate = false;
                }
            }

            void rotate(const glm::vec3& eulerAngles,
                const glm::vec3& customAxisX = glm::vec3(1.0f, 0.0f, 0.0f),
                const glm::vec3& customAxisY = glm::vec3(0.0f, 1.0f, 0.0f),
                const glm::vec3& customAxisZ = glm::vec3(0.0f, 0.0f, 1.0f))
            {
                if (eulerAngles == glm::vec3(0.0f)) return;

                glm::vec3 radians = glm::radians(eulerAngles);
                glm::quat rotationX = glm::angleAxis(radians.x, customAxisX);
                glm::quat rotationY = glm::angleAxis(radians.y, customAxisY);
                glm::quat rotationZ = glm::angleAxis(radians.z, customAxisZ);

                rotation = glm::normalize(rotationY * rotationX * rotationZ * rotation);

                needsUpdate = true;
            }

            bool areAligned(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.001f)
            {
                glm::vec3 normA = glm::normalize(a);
                glm::vec3 normB= glm::normalize(b);

                float dotProduct = glm::dot(normA, normB);

                return glm::epsilonEqual(dotProduct, 1.0f, epsilon);
            }
    };
}

#endif // !TRANSFORM_H