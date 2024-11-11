#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/ECS/IComponent.hpp>

struct Transform : Essentia::IComponent
{
    private:
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::mat4 modelMatrix;
        bool needsUpdate;

    public:
        Transform(): 
            position(0.0f, 0.0f, 0.0f),
            rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
            scale(1.0f, 1.0f, 1.0f),
            modelMatrix(1.0f),
            needsUpdate(false) {}

        Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl):
            position(pos),
            rotation(rot),
            scale(scl),
            modelMatrix(1.0f),
            needsUpdate(true) 
            {updateMatrix();}

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
                modelMatrix =   glm::translate(glm::mat4(1.0f), position) *
                                glm::mat4_cast(rotation) *
                                glm::scale(glm::mat4(1.0f), scale);

                needsUpdate = false;
            }
        }
};

#endif // !TRANSFORM_H
