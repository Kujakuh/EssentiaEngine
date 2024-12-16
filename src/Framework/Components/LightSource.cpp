#include <Framework/Components/LightSource.hpp>

namespace Essentia
{
    LightSource::LightSource()
        : direction(glm::vec3(0.0f, -1.0f, 0.0f)), color(glm::vec3(1.0)),
        ambient(glm::vec3(0.2f)), diffuse(glm::vec3(0.5f)), specular(glm::vec3(1.0f)),
        intensity(0.1f), type(LightType::Point),
        innerCutOff(0.0f), outerCutOff(0.0f),
        constant(1.0f), linear(0.09f), quadratic(0.032f), needsUpdate(true) {}

    LightSource::LightSource(LightType lightType) : LightSource() {
        type = lightType;
        if (lightType == LightType::Directional) {
            direction = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        else if (lightType == LightType::Spot) {
            direction = glm::vec3(0.0f, -1.0f, 0.0f);
            innerCutOff = glm::cos(glm::radians(12.5f));
            outerCutOff = glm::cos(glm::radians(15.0f));
        }
        needsUpdate = true;
    }

    LightSource LightSource::pointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity) {
        LightSource light;
        light.type = LightType::Point;
        light.ambient = ambient;
        light.diffuse = diffuse;
        light.specular = specular;
        light.intensity = intensity;
        light.constant = 1.0f;
        light.linear = 0.09f;
        light.quadratic = 0.032f;
        light.needsUpdate = true;
        return light;
    }

    LightSource LightSource::directionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity) {
        LightSource light;
        light.type = LightType::Directional;
        light.direction = direction;
        light.ambient = ambient;
        light.diffuse = diffuse;
        light.specular = specular;
        light.intensity = intensity;
        light.needsUpdate = true;
        return light;
    }

    LightSource LightSource::spotLight(const glm::vec3& direction, float innerCutOff, float outerCutOff, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float intensity) {
        LightSource light;
        light.type = LightType::Spot;
        light.direction = direction;
        light.innerCutOff = innerCutOff;
        light.outerCutOff = outerCutOff;
        light.ambient = ambient;
        light.diffuse = diffuse;
        light.specular = specular;
        light.intensity = intensity;
        light.constant = 1.0f;
        light.linear = 0.09f;
        light.quadratic = 0.032f;
        light.needsUpdate = true;
        return light;
    }

    const glm::vec3& LightSource::GetDirection() const { return direction; }
    void LightSource::SetDirection(const glm::vec3& dir) { direction = dir; }

    const glm::vec3& LightSource::GetColor() const { return color; }
    void LightSource::SetColor(const glm::vec3& col) { color = col; needsUpdate = true; }

    const glm::vec3& LightSource::GetAmbient() const { return ambient; }
    void LightSource::SetAmbient(const glm::vec3& amb) { ambient = amb; needsUpdate = true;}

    const glm::vec3& LightSource::GetDiffuse() const { return diffuse; }
    void LightSource::SetDiffuse(const glm::vec3& diff) { diffuse = diff; needsUpdate = true;}

    const glm::vec3& LightSource::GetSpecular() const { return specular; }
    void LightSource::SetSpecular(const glm::vec3& spec) { specular = spec; needsUpdate = true; }

    float LightSource::GetIntensity() const { return intensity; }
    void LightSource::SetIntensity(float value) { intensity = value; needsUpdate = true; }

    LightType LightSource::GetType() const { return type; }

    float LightSource::GetInnerCutOff() const { return innerCutOff; }
    void LightSource::SetInnerCutOff(float cutOff) { innerCutOff = cutOff; needsUpdate = true;}

    float LightSource::GetOuterCutOff() const { return outerCutOff; }
    void LightSource::SetOuterCutOff(float cutOff) { outerCutOff = cutOff; needsUpdate = true;}

    float LightSource::GetConstant() const { return constant; }
    void LightSource::SetConstant(float value) { constant = value; needsUpdate = true;}

    float LightSource::GetLinear() const { return linear; }
    void LightSource::SetLinear(float value) { linear = value; needsUpdate = true;}

    float LightSource::GetQuadratic() const { return quadratic; }
    void LightSource::SetQuadratic(float value) { quadratic = value; needsUpdate = true;}

}