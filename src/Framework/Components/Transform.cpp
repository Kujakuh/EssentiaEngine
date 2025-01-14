#include <Framework/Components/Transform.hpp>

namespace Essentia
{
    Transform::Transform() :
        position(0.0f, 0.0f, 0.0f),
        rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        scale(1.0f, 1.0f, 1.0f),
        modelMatrix(1.0f),
        needsUpdate(false) {}

    Transform::Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl) :
        position(pos),
        rotation(rot),
        scale(scl),
        modelMatrix(1.0f),
        needsUpdate(true)
    {
        updateMatrix();
    }

    void Transform::setPosition(const glm::vec3& pos) { position = pos; needsUpdate = true; }
    void Transform::setRotation(const glm::quat& rot) { rotation = rot; needsUpdate = true; }
    void Transform::setScale(const glm::vec3& scl) { scale = scl; needsUpdate = true; }

    glm::vec3& Transform::setPosition() { needsUpdate = true; return position; }
    glm::quat& Transform::setRotation() { needsUpdate = true; return rotation; }
    glm::vec3& Transform::setScale() { needsUpdate = true; return scale; }

    glm::vec3 Transform::getPosition() const { return position; }
    glm::quat Transform::getRotation() const { return rotation; }
    glm::vec3 Transform::getScale() const { return scale; }
    glm::mat4 Transform::getModelMatrix() const { return modelMatrix; }

    void Transform::updateMatrix()
    {
        if (needsUpdate)
        {
            modelMatrix = glm::translate(glm::mat4(1.0f), position) *
                glm::mat4_cast(rotation) *
                glm::scale(glm::mat4(1.0f), scale);

            needsUpdate = false;
        }
    }

    void Transform::rotate(const glm::vec3& eulerAngles, bool defautAxis,
        const glm::vec3& customAxisX,
        const glm::vec3& customAxisY,
        const glm::vec3& customAxisZ)
    {
        if (eulerAngles == glm::vec3(0.0f)) return;

        glm::vec3 radians = glm::radians(eulerAngles);
        glm::quat rotationX = glm::angleAxis(radians.x, customAxisX);
        glm::quat rotationY = glm::angleAxis(radians.y, customAxisY);
        glm::quat rotationZ = glm::angleAxis(radians.z, customAxisZ);

        rotation = glm::normalize(rotationY * rotationX * rotationZ * rotation);

        needsUpdate = true;
    }

    void Transform::rotate(const glm::vec3& eulerAngles, const glm::vec3& centerPoint) {
        if (eulerAngles == glm::vec3(0.0f)) return;

        // Convertimos los ángulos de Euler a radianes
        glm::vec3 radians = glm::radians(eulerAngles);

        // Creamos quaternions para cada eje
        glm::quat rotationX = glm::angleAxis(radians.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotationY = glm::angleAxis(radians.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat rotationZ = glm::angleAxis(radians.z, glm::vec3(0.0f, 0.0f, 1.0f));

        // Combinamos las rotaciones
        glm::quat combinedRotation = glm::normalize(rotationY * rotationX * rotationZ);

        // Trasladamos el sistema de coordenadas al punto de rotación
        glm::vec3 translatedPosition = position - centerPoint;

        // Aplicamos la rotación
        translatedPosition = combinedRotation * translatedPosition;

        // Revertimos la traslación
        position = translatedPosition + centerPoint;

        // Actualizamos la rotación acumulada
        rotation = glm::normalize(combinedRotation * rotation);

        // Indicamos que la matriz necesita actualizarse
        needsUpdate = true;
    }


    bool Transform::areAligned(const glm::vec3& a, const glm::vec3& b, float epsilon)
    {
        glm::vec3 normA = glm::normalize(a);
        glm::vec3 normB = glm::normalize(b);

        return glm::epsilonEqual(glm::dot(normA, normB), 1.0f, epsilon);
    }
}
