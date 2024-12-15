#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <Core/ECS/IComponent.hpp>
#include <Shared/enums.hpp>

#include <glm/glm.hpp>

namespace Essentia {

    class LightSource : public IComponent
    {
        private:
            LightType type;

            glm::vec3 direction;
            glm::vec3 color;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;

            float intensity;

            float innerCutOff;
            float outerCutOff;

            float constant;
            float linear;
            float quadratic;

        public:
            LightSource();
            LightSource(LightType lightType);

            static LightSource pointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity = 0.1f);
            static LightSource directionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity = 0.1f);
            static LightSource spotLight(const glm::vec3& direction, float innerCutOff, float outerCutOff, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity = 0.1f);

            const glm::vec3& GetColor() const;
            void SetColor(const glm::vec3& dir);

            const glm::vec3& GetDirection() const;
            void SetDirection(const glm::vec3& dir);

            const glm::vec3& GetAmbient() const;
            void SetAmbient(const glm::vec3& amb);

            const glm::vec3& GetDiffuse() const;
            void SetDiffuse(const glm::vec3& diff);

            const glm::vec3& GetSpecular() const;
            void SetSpecular(const glm::vec3& spec);

            float GetIntensity() const;
            void SetIntensity(float value);

            LightType GetType() const;

            float GetInnerCutOff() const;
            void SetInnerCutOff(float cutOff);

            float GetOuterCutOff() const;
            void SetOuterCutOff(float cutOff);

            float GetConstant() const;
            void SetConstant(float value);

            float GetLinear() const;
            void SetLinear(float value);

            float GetQuadratic() const;
            void SetQuadratic(float value);
    };

}

#endif // !LIGHTSOURCE_H