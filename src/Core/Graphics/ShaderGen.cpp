#include <Core/Graphics/ShaderGen.hpp>

ShaderGenerator::ShaderGenerator()
{
    if (GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
    {
        versionNextensionsHeader = R"(
        #version 440 core
        #extension GL_ARB_bindless_texture : require
        )";
    }
    else
    {
        versionNextensionsHeader = R"(
        #version 330 core
        )";
    }
    shaderHeaders2D[VERTEX] = "";
    shaderHeaders2D[FRAGMENT] = "";
    shaderHeaders2D[GEOMETRY] = "";

    shaderHeaders3D[VERTEX] = "";
    shaderHeaders3D[FRAGMENT] = "";
    shaderHeaders3D[GEOMETRY] = "";

    customFunctions[VERTEX] = {};
    customFunctions[FRAGMENT] = {};
    customFunctions[GEOMETRY] = {};

    customMainCode[VERTEX] = "";
    customMainCode[FRAGMENT] = "";
    customMainCode[GEOMETRY] = "";

    // Header para vertex shader (simple 2D)
    shaderHeaders2D[VERTEX] = R"(
    layout(location = 0) in vec3 aPos;
    layout(location = 2) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    )";

    // Header para fragment shader (base)
    shaderHeaders2D[FRAGMENT] = R"(
    in vec2 TexCoord;

    out vec4 FragColor;
    uniform Material material;
    )";

    // Header para geometry shader (si se necesita)
    shaderHeaders2D[GEOMETRY] = R"(
    layout(points) in;
    layout(triangle_strip, max_vertices = 4) out;

    void main() {
    )";

    shaderHeaders3D[VERTEX] = R"(
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in vec3 aNormal;
    layout(location = 2) in vec2 aTexCoord;
    layout(location = 3) in vec3 aTangent;
    layout(location = 4) in vec3 aBitangent;
    layout(location = 5) in ivec4 aBoneIDs;
    layout(location = 6) in vec4 aWeights;

    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoord;
    out vec3 Tangent;
    out vec3 Bitangent;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    )";

    shaderHeaders3D[FRAGMENT] = R"(
    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;
    in vec3 Tangent;
    in vec3 Bitangent;

    out vec4 FragColor;

    uniform Material material;
    uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);

    struct Light {
        vec3 position;
        vec3 direction;
        float innerCutOff;
        float outerCutOff;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    uniform Light lights[10];
    uniform int lightsNum = 0;

    vec3 bpSpecularLight(vec3 normal, vec3 lightDir, vec3 viewDir, float shininess)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir); // Halfway vector
        float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        return spec * vec3(1.0); // Specular color (white for simplicity)
    }

    vec3 dirLight(Light light, vec3 fragPos, vec3 normal, vec3 viewPos) {
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb * material.color;
        return diffuse;
    }

    vec3 pointLight(Light light, vec3 fragPos, vec3 normal, vec3 viewPos) {
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);

        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 diffuse = light.diffuse * diff * attenuation * texture(material.diffuse, TexCoord).rgb * material.color;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 specular = bpSpecularLight(normal, lightDir, viewDir, material.shininess) *
                        texture(material.specular, TexCoord).rgb * attenuation;
        return diffuse + specular;
    }

    vec3 spotLight(Light light, vec3 fragPos, vec3 normal, vec3 viewPos) {
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);

        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.innerCutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        vec3 diffuse = light.diffuse * diff * intensity * attenuation * texture(material.diffuse, TexCoord).rgb * material.color;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 specular = bpSpecularLight(normal, lightDir, viewDir, material.shininess) *
                        texture(material.specular, TexCoord).rgb * intensity * attenuation;
        return diffuse + specular;
    }
    )";
}

void ShaderGenerator::addTextureUniform(const std::string& textureFileName) 
{
    textureUniforms.push_back(textureFileName);
}
void ShaderGenerator::removeTextureUniform(const std::string& textureUniformName)
{
    auto it = std::find(textureUniforms.begin(), textureUniforms.end(), textureUniformName);
    if (it != textureUniforms.end()) textureUniforms.erase(it);
}
void ShaderGenerator::addCustomFunction(SH_TYPE type, const std::string& functionCode)
{
    customFunctions[type].push_back(functionCode);
}
void ShaderGenerator::addCustomFunctionFromFile(SH_TYPE type, const std::string& filePath)
{
    customFunctions[type].push_back(loadFromFile(filePath));
}
void ShaderGenerator::addMainCode(SH_TYPE type, const std::string& mainCode)
{
    customMainCode[type] += mainCode + "\n";
}
void ShaderGenerator::addMainCodeFromFile(SH_TYPE type, const std::string& filePath)
{
    customMainCode[type] += loadFromFile(filePath) + "\n";
}

std::string ShaderGenerator::generateShader2D(SH_TYPE type) const
{
    std::ostringstream shader;
    shader << versionNextensionsHeader << "\n";
    if (type == FRAGMENT)
    {
        if (GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
        {
            shader << R"(
            struct Material {
                layout(bindless_sampler) sampler2D diffuse;
                layout(bindless_sampler) sampler2D specular;
                layout(bindless_sampler) sampler2D normal;
                layout(bindless_sampler) sampler2D height;
                float shininess;
                vec3 color;
            };
            )";
        }
        else
        {
            shader << R"(
            struct Material {
                sampler2D diffuse;
                sampler2D specular;
                sampler2D normal;
                sampler2D height;
                float shininess;
                vec3 color;
            };
            )";
        }
    }
    shader << shaderHeaders2D.at(type) << "\n";

    // Add Texture Uniforms declarations
    if (type == FRAGMENT) 
    {
        if(GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
            for (const auto& uniform : textureUniforms)
                shader << "layout(bindless_sampler) uniform sampler2D " << uniform << ";\n";
        else
            for (const auto& uniform : textureUniforms)
                shader << "uniform sampler2D "  << uniform << ";\n";
    }

    // Add pre-main content (custom functions and others)
    if (!customFunctions.at(type).empty())
    {
        for (const auto& function : customFunctions.at(type))
            shader << function << "\n";
    }

    shader << R"(
    void main() {
    )";

    if (customMainCode.at(type).empty()) {
        if (type == FRAGMENT)
        {
            shader << "    FragColor = texture(material.diffuse, TexCoord);\n";
        }
        else if (type == VERTEX)
        {
            shader << "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n        TexCoord = aTexCoord;\n";
        }
    }
    else shader << customMainCode.at(type); // Add any custom main code

    shader << R"(
    }
    )";

    // ----------------- DEBUG -----------------
    std::cout << shader.str() << "\n";
    // ----------------- DEBUG -----------------

    return shader.str();
}

std::string ShaderGenerator::generateShader3D(SH_TYPE type, bool ambientLightOn) const
{
    std::ostringstream shader;

    shader << versionNextensionsHeader << "\n";
    if (type == FRAGMENT) 
    {
        if (GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
        {
            shader << R"(
            struct Material {
                layout(bindless_sampler) sampler2D diffuse;
                layout(bindless_sampler) sampler2D specular;
                layout(bindless_sampler) sampler2D normal;
                layout(bindless_sampler) sampler2D height;
                float shininess;
                vec3 color;
            };
            )";
        }
        else
        {
            shader << R"(
            struct Material {
                sampler2D diffuse;
                sampler2D specular;
                sampler2D normal;
                sampler2D height;
                float shininess;
                vec3 color;
            };
            )";
        }
    }
    shader << shaderHeaders3D.at(type) << "\n";

    // Uniforms de texturas
    if (type == FRAGMENT)
    {
        if (GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
        {
            for (const auto& uniform : textureUniforms)
                shader << "     layout(bindless_sampler) uniform sampler2D " << uniform << ";\n";
        }
        else
        {
            for (const auto& uniform : textureUniforms)
                shader << "     uniform sampler2D " << uniform << ";\n";
        }
    }

    // Funciones personalizadas
    for (const auto& function : customFunctions.at(type))
    {
        shader << function << "\n";
    }

    shader << R"(
    void main() {
    )";

    if (type == FRAGMENT)
    {
        shader << R"(
        vec3 norm = normalize(Normal);
        vec3 diffuse = vec3(0.0);
        vec3 ambient = texture(material.diffuse, TexCoord).rgb;
        vec3 specular = vec3(0.0);

        )";

        if(ambientLightOn)
            shader << R"(
            Light sunLight = Light(
            vec3(0.0, 0.0, 0.0), // Posición no relevante para luz direccional
            vec3(0.0, -1.0, 0.0), // Dirección de la luz, emula el sol hacia abajo
            0.0, // InnerCutOff no necesario para luces direccionales
            0.0, // OuterCutOff no necesario para luces direccionales
            vec3(0.3, 0.3, 0.3), // Luz ambiental tenue
            vec3(1.0, 1.0, 1.0), // Luz difusa fuerte (luz del sol)
            vec3(1.0, 1.0, 1.0), // Luz especular fuerte
            1.0, // Constante (luz direccional no cambia con la distancia)
            0.0, // Linear (sin caída de luz con la distancia)
            0.0  // Quadratic (sin caída de luz con la distancia)
            );

            ambient *= sunLight.ambient;
            diffuse += dirLight(sunLight, FragPos, norm, viewPos);
            specular += bpSpecularLight(norm, normalize(sunLight.direction), normalize(viewPos - FragPos), 16.0);
            )";

        if (customMainCode.at(type) != "")
        {
            // Any change that want to be made to the final pixel must be end up being applied to "result" variable
            shader << customMainCode.at(type) << "\n";
        }

        shader << R"(
        for (int i = 0; i < lightsNum; ++i) {
            Light light = lights[i];

            // Luz direccional
            if (length(light.position) == 0.0) {
                diffuse += dirLight(light, FragPos, norm, viewPos);
                ambient += light.ambient;
            }
            // Luz puntual
            else if (light.innerCutOff > 0.0) {
                diffuse += pointLight(light, FragPos, norm, viewPos);
                ambient += light.ambient;
            }
            // Luz focal
            else {
                diffuse += spotLight(light, FragPos, norm, viewPos);
                ambient += light.ambient;
            }
        }
        
        ambient = min(ambient, vec3(0.3));

        vec4 result = vec4(diffuse + ambient, 1.0);
        )";

       /* if (!textureUniforms.empty())
        {
            if (textureUniforms.size() == 1)
            {
                shader << R"(vec4 texColor = texture()" << textureUniforms[0] << ", TexCoord);\n";
            }
            else
            {
                shader << R"(vec4 texColor = vec4(1.0);
                )";
                for (const auto& uniform : textureUniforms)
                {
                    shader << "texColor *= texture(" << uniform << ", TexCoord);\n";
                }
            }

            shader << R"(
            res *= texColor;
            )";
        }
        else shader << "res = material.color;\n";*/

        shader << R"(
        FragColor = result;
        )";
    }
    else if (type == VERTEX)
    {
        if (customMainCode.at(type) != "")
        {
            shader << customMainCode.at(type) << "\n";
        }
        else 
        {
            shader << R"(
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoord = aTexCoord;
            Tangent = aTangent;
            Bitangent = aBitangent;
            gl_Position = projection * view * vec4(FragPos, 1.0);
            )";
        }
    }

    shader << R"(
    }
    )";

    // ----------------- DEBUG -----------------
    std::cout << shader.str() << "\n";
    // ----------------- DEBUG -----------------

    return shader.str();
}


std::string ShaderGenerator::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open()) throw std::runtime_error("No se pudo abrir el archivo: " + filePath);

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}