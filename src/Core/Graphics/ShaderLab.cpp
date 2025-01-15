#include <Core/Graphics/ShaderLab.hpp>

namespace Essentia
{
    ShaderLab::ShaderLab()
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

        if (renderMode == RENDER_MODE::PONG_SHADING)
            shaderHeaders3D[FRAGMENT] = R"(

            #define PI 3.1415927

            in vec3 FragPos;
            in vec3 Normal;
            in vec2 TexCoord;
            in vec3 Tangent;
            in vec3 Bitangent;

            out vec4 FragColor;

            uniform Material material;
            uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);

            struct Light {
                int type; // 0 = puntual, 1 = direccional, 2 = focal

                vec3 position;
                vec3 direction;
                vec3 color;

                float intensity;

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

            vec3 calculateAmbient(Light light) {
                return light.ambient * texture(material.diffuse, TexCoord).rgb * material.color;
            }

            vec3 calculateDiffuse(Light light, vec3 normal, vec3 lightDir) {
                float diff = max(dot(normal, lightDir), 0.0);
                return light.diffuse * diff * texture(material.diffuse, TexCoord).rgb * material.color;
            }

            vec3 calculateSpecular(Light light, vec3 normal, vec3 lightDir, vec3 viewDir) {
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                return light.specular * spec * texture(material.specular, TexCoord).rgb;
            }

            vec3 calculatePointLight(Light light, vec3 fragPos, vec3 normal, vec3 viewPos) {
                vec3 lightDir = normalize(light.position - fragPos);
                float distance = length(light.position - fragPos);
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

                vec3 ambient = calculateAmbient(light) * attenuation * light.intensity;
                vec3 diffuse = calculateDiffuse(light, normal, lightDir) * attenuation * light.intensity;
                vec3 viewDir = normalize(viewPos - fragPos);
                vec3 specular = calculateSpecular(light, normal, lightDir, viewDir) * attenuation * light.intensity;

                return ambient + diffuse + specular;
            }

            vec3 calculateDirectionalLight(Light light, vec3 normal, vec3 viewPos) {
                vec3 lightDir = normalize(-light.direction);

                vec3 ambient = calculateAmbient(light) * light.intensity;
                vec3 diffuse = calculateDiffuse(light, normal, lightDir) * light.intensity;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 specular = calculateSpecular(light, normal, lightDir, viewDir) * light.intensity;

                return ambient + diffuse + specular;
            }

            vec3 calculateSpotLight(Light light, vec3 fragPos, vec3 normal, vec3 viewPos) {
                vec3 lightDir = normalize(light.position - fragPos);
                float theta = dot(lightDir, normalize(-light.direction));
                float epsilon = light.innerCutOff - light.outerCutOff;
                float spotlightFactor = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

                float distance = length(light.position - fragPos);
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

                vec3 ambient = calculateAmbient(light) * attenuation * spotlightFactor * light.intensity;
                vec3 diffuse = calculateDiffuse(light, normal, lightDir) * attenuation * spotlightFactor * light.intensity;
                vec3 viewDir = normalize(viewPos - fragPos);
                vec3 specular = calculateSpecular(light, normal, lightDir, viewDir) * attenuation * spotlightFactor * light.intensity;

                return ambient + diffuse + specular;
            }

            vec3 getNormalFromMap()
            {
                vec3 tangentNormal = texture(material.normal, TexCoord).xyz * 2.0 - 1.0;
                // Verificamos si el normal está correctamente mapeado
                if (length(tangentNormal) == 0.0)
                    tangentNormal = Normal * 2.0 - 1.0;

                vec3 Q1  = dFdx(FragPos);
                vec3 Q2  = dFdy(FragPos);
                vec2 st1 = dFdx(TexCoord);
                vec2 st2 = dFdy(TexCoord);

                vec3 N   = normalize(Normal);
                vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
                vec3 B  = -normalize(cross(N, T));
                mat3 TBN = mat3(T, B, N);

                return normalize(TBN * tangentNormal);
            }
            )";

        if (renderMode == RENDER_MODE::PBR)
            shaderHeaders3D[FRAGMENT] = R"(
            #define PI 3.1415927

            in vec3 FragPos;
            in vec3 Normal;
            in vec2 TexCoord;
            in vec3 Tangent;
            in vec3 Bitangent;

            out vec4 FragColor;

            uniform Material material;
            uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);

            struct Light {
                int type; // 0 = puntual, 1 = direccional, 2 = focal

                vec3 position;
                vec3 direction;
                vec3 color;
                
                float constant;
                float linear;
                float quadratic;

                float intensity;

                float innerCutoff;
                float outerCutoff;
            };

            uniform Light lights[100];
            uniform int lightsNum = 0;

            // ----------------------------------------------------------------------------
            float calculateAttenuation(vec3 fragPos, Light light)
            {
                if (light.type == 0 || light.type == 2) 
                {
                    float distance = length(light.position - fragPos);

                    float att = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

                    //float alpha = 0.1;  // Factor de escala
                    //float beta = 2.0;   // Exponente para controlar la rapidez de la atenuación
                    //float customAtt = 1.0 / (1.0 + alpha * pow(distance, beta));

                    return att;
                }

                return 1.0;
            }
            // ----------------------------------------------------------------------------
            vec3 calculateLightDir(vec3 fragPos, Light light)
            {
                if (light.type == 1) 
                {
                    // Luz direccional
                    return -light.direction;
                } 
                else 
                    return light.position - fragPos;
            }
            // ----------------------------------------------------------------------------
            float calculateSpotEffect(vec3 lightDir, Light light)
            {
                if (light.type != 2) return 1.0; // No es luz focal
                float theta = dot(lightDir, normalize(-light.direction));
                float epsilon = light.innerCutoff - light.outerCutoff;
                return clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
            }
            // ----------------------------------------------------------------------------
            vec3 getNormalFromMap()
            {
                vec3 tangentNormal = texture(material.normal, TexCoord).xyz * 2.0 - 1.0;
                // Verificamos si el normal está correctamente mapeado
                if (length(tangentNormal) == 0.0)
                    tangentNormal = Normal * 2.0 - 1.0;

                vec3 Q1  = dFdx(FragPos);
                vec3 Q2  = dFdy(FragPos);
                vec2 st1 = dFdx(TexCoord);
                vec2 st2 = dFdy(TexCoord);

                vec3 N   = normalize(Normal);
                vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
                vec3 B  = -normalize(cross(N, T));
                mat3 TBN = mat3(T, B, N);

                return normalize(TBN * tangentNormal);
            }
            // ----------------------------------------------------------------------------
            float DistributionGGX(vec3 N, vec3 H, float roughness)
            {
                float a = roughness*roughness;
                float a2 = a*a;
                float NdotH = max(dot(N, H), 0.0);
                float NdotH2 = NdotH*NdotH;

                float nom   = a2;
                float denom = (NdotH2 * (a2 - 1.0) + 1.0);
                denom = PI * denom * denom;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            float GeometrySchlickGGX(float NdotV, float roughness)
            {
                float r = (roughness + 1.0);
                float k = (r*r) / 8.0;

                float nom   = NdotV;
                float denom = NdotV * (1.0 - k) + k;

                return nom / denom;
            }
            // ----------------------------------------------------------------------------
            float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
            {
                float NdotV = max(dot(N, V), 0.0);
                float NdotL = max(dot(N, L), 0.0);
                float ggx2 = GeometrySchlickGGX(NdotV, roughness);
                float ggx1 = GeometrySchlickGGX(NdotL, roughness);

                return ggx1 * ggx2;
            }
            // ----------------------------------------------------------------------------
            vec3 fresnelSchlick(float cosTheta, vec3 F0)
            {
                return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
            }
            // ----------------------------------------------------------------------------
            vec3 calculatePBR(vec3 normal, vec3 viewDir, vec3 lightDir, Material material, Light light, vec2 TexCoord, vec3 fragPos) 
            {
                vec4 albedoSample = texture(material.diffuse, vec2(TexCoord.x, TexCoord.y)); // RGBA
                vec3 albedo = (albedoSample.rgb != vec3(0.0)) ? albedoSample.rgb : material.color;
                albedo = pow(albedo, vec3(2.2));  // Corrección gamma de la textura
                
                float metallic = texture(material.metallic, vec2(TexCoord.x, TexCoord.y)).r;
                float roughness = texture(material.roughness, vec2(TexCoord.x, TexCoord.y)).r;

                float ao = texture(material.ao, vec2(TexCoord.x, TexCoord.y)).r;
                if (isnan(ao)) ao = 1.0;

                vec3 N = normal;  
                vec3 V = viewDir;

                vec3 F0 = vec3(0.03); 
                F0 = mix(F0, albedo, metallic);

                vec3 L = normalize(lightDir);
                vec3 H = normalize(V + L);
                float attenuation = calculateAttenuation(fragPos, light);
                vec3 radiance = light.color * attenuation * light.intensity;

                float spotEffect = calculateSpotEffect(L, light);
                radiance *= spotEffect;

                float NDF = DistributionGGX(N, H, roughness);   
                float G = GeometrySmith(N, V, L, roughness);      
                vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

                vec3 numerator = NDF * G * F; 
                float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.00001;
                vec3 specular = numerator / denominator;

                vec3 kS = F;
                vec3 kD = vec3(1.0) - kS;
                kD *= 1.0 - metallic;

                float NdotL = max(dot(N, L), 0.0);
                vec3 nan =  texture(material.normal, TexCoord).xyz;
                if(length(nan) == 0) { NdotL = 1.0 - NdotL; }

                // Radiancia total
                vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
                // Iluminación ambiental
                vec3 ambient = vec3(0.03) * albedo * ao;

                vec3 color = ambient + Lo;

                // Tonemapping HDR
                color = color / (color + vec3(1.0));
                // Corrección gamma
                color = pow(color, vec3(1.0 / 2.2));

                return color;
            }
            )";
    }

    void ShaderLab::addTextureUniform(const std::string& textureFileName)
    {
        textureUniforms.push_back(textureFileName);
    }
    void ShaderLab::removeTextureUniform(const std::string& textureUniformName)
    {
        auto it = std::find(textureUniforms.begin(), textureUniforms.end(), textureUniformName);
        if (it != textureUniforms.end()) textureUniforms.erase(it);
    }
    void ShaderLab::addCustomFunction(SH_TYPE type, const std::string& functionCode)
    {
        customFunctions[type].push_back(functionCode);
    }
    void ShaderLab::addCustomFunctionFromFile(SH_TYPE type, const std::string& filePath)
    {
        customFunctions[type].push_back(loadFromFile(filePath));
    }
    void ShaderLab::addMainCode(SH_TYPE type, const std::string& mainCode)
    {
        customMainCode[type] += mainCode + "\n";
    }
    void ShaderLab::addMainCodeFromFile(SH_TYPE type, const std::string& filePath)
    {
        customMainCode[type] += loadFromFile(filePath) + "\n";
    }

    std::string ShaderLab::generateShader2D(SH_TYPE type) const
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
                    layout(bindless_sampler) sampler2D alpha;
                    layout(bindless_sampler) sampler2D metallic;
                    layout(bindless_sampler) sampler2D roughness;
                    layout(bindless_sampler) sampler2D ao;
                    layout(bindless_sampler) sampler2D emissive;
                    float shininess;
                    vec3 color;
                    float metallicValue;
                    float roughnessValue;
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
                    sampler2D alpha;
                    sampler2D metallic;
                    sampler2D roughness;
                    sampler2D ao;
                    sampler2D emissive;
                    float shininess;
                    vec3 color;
                    float metallicValue;
                    float roughnessValue;
                };
                )";
            }
        }
        shader << shaderHeaders2D.at(type) << "\n";

        // Add Texture Uniforms declarations
        if (type == FRAGMENT)
        {
            if (GLAD_GL_ARB_bindless_texture && Essentia::bindlessTexturesMode)
                for (const auto& uniform : textureUniforms)
                    shader << "layout(bindless_sampler) uniform sampler2D " << uniform << ";\n";
            else
                for (const auto& uniform : textureUniforms)
                    shader << "uniform sampler2D " << uniform << ";\n";
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
                shader << "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n";
                shader << "    TexCoord = aTexCoord;\n";
            }
        }
        else shader << customMainCode.at(type);

        shader << R"(
        }
        )";

        // ----------------- DEBUG -----------------
        //std::cout << shader.str() << "\n";
        // ----------------- DEBUG -----------------

        return shader.str();
    }

    std::string ShaderLab::generateShader3D(SH_TYPE type, bool ambientLightOn) const
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
                    layout(bindless_sampler) sampler2D alpha;
                    layout(bindless_sampler) sampler2D metallic;
                    layout(bindless_sampler) sampler2D roughness;
                    layout(bindless_sampler) sampler2D ao;
                    layout(bindless_sampler) sampler2D emissive;
                    float shininess;
                    vec3 color;
                    float metallicValue;
                    float roughnessValue;
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
                    sampler2D alpha;
                    sampler2D metallic;
                    sampler2D roughness;
                    sampler2D ao;
                    sampler2D emissive;
                    float shininess;
                    vec3 color;
                    float metallicValue;
                    float roughnessValue;
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

        for (const auto& function : customFunctions.at(type))
        {
            shader << function << "\n";
        }

        shader << R"(
        void main() {
        )";

        if (type == FRAGMENT)
        {
            if (renderMode == RENDER_MODE::PONG_SHADING)
            {
                shader << R"(
                vec3 norm;
                if (length(texture(material.normal, TexCoord)) != 0) 
                {
                    norm = normalize(getNormalFromMap());
                } 
                else 
                {
                    norm = normalize(Normal);
                }

                vec3 rest = vec3(0.0);
                )";

                if (ambientLightOn)
                    shader << R"(
                    Light sunLight;
                    sunLight.type = 1; // Luz direccional
                    sunLight.position = vec3(0.0, 0.0, 0.0); // La posición no es relevante para luz direccional
                    sunLight.direction = normalize(vec3(0.0, -1.0, 0.0)); // Emula el sol iluminando hacia abajo
                    sunLight.color = vec3(1.0, 1.0, 1.0); // Luz blanca pura
                    sunLight.intensity = 1.0; // Intensidad estándar

                    sunLight.ambient = vec3(0.05, 0.05, 0.05); // Luz ambiental tenue
                    sunLight.diffuse = vec3(0.5, 0.5, 0.5); // Luz difusa intensa para simular luz solar
                    sunLight.specular = vec3(1.0, 1.0, 1.0); // Especular fuerte para reflejos brillantes

                    sunLight.innerCutOff = 0.0; // No aplica para luz direccional
                    sunLight.outerCutOff = 0.0; // No aplica para luz direccional

                    sunLight.constant = 1.0; // No afecta para luz direccional
                    sunLight.linear = 0.0; // No afecta para luz direccional
                    sunLight.quadratic = 0.0; // No afecta para luz direccional

                    rest += calculateDirectionalLight(sunLight, norm, viewPos);
                    )";
                else
                    shader << R"(
                    rest = vec3(1.0);
                    )";

                shader << R"(

                for (int i = 0; i < lightsNum; i++) {
                    if (lights[i].type == 0) {
                        rest *= calculatePointLight(lights[i], FragPos, norm, viewPos);
                    } else if (lights[i].type == 1) {
                        rest *= calculateDirectionalLight(lights[i], norm, viewPos);
                    } else if (lights[i].type == 2) {
                        rest *= calculateSpotLight(lights[i], FragPos, norm, viewPos);
                    }
                }

                vec4 alphaMap = texture(material.alpha, TexCoord);
                float alphaValue = alphaMap.r; // Suponiendo que el canal rojo representa la opacidad

                float diff = texture(material.diffuse, TexCoord).a;

                // Ajustar el canal alfa de ambient si alphaMap tiene datos válidos
                if (alphaValue > 0.0) {
                    diff *= alphaValue;
                }

                // Si diff sigue siendo 0, usar el valor de alphaMap si está disponible
                if (diff == 0.0 && alphaMap.a > 0.0) {
                    diff = alphaValue;
                }

                vec4 result = vec4(rest, diff);
                )";
            }
            if (renderMode == RENDER_MODE::PBR)
            {
                shader << R"(
                vec3 norm = getNormalFromMap();

                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 res = vec3(1.0);

                )";

                if (!ambientLightOn)
                    shader << R"(
                    // Luz solar base (luz direccional)
                    Light sunLight;
                    sunLight.type = 1; // Luz direccional
                    sunLight.direction = normalize(vec3(0.0, -1.0, 0.0)); // Dirección del sol (ligeramente inclinada)
                    sunLight.color = vec3(1.0, 0.95, 0.9); // Color cálido, similar a la luz solar
                    sunLight.intensity = 10.0; // Intensidad más alta que las luces estándar

                    // Cálculo de la luz solar base
                    vec3 lightDir = calculateLightDir(FragPos, sunLight);
                    res *= calculatePBR(norm, viewDir, lightDir, material, sunLight, TexCoord, FragPos);
                    )";

                shader << R"(
                
                for (int i = 0; i < lightsNum; ++i)
                {
                    vec3 lightDir = calculateLightDir(FragPos, lights[i]);
                    res *= calculatePBR(norm, viewDir, lightDir, material, lights[i], TexCoord, FragPos);
                }

                vec4 albedoSample = texture(material.diffuse, TexCoord);
                float opacityMap = texture(material.alpha, TexCoord).r;
                float opacity = (opacityMap > 0.0) ? opacityMap : (albedoSample.a > 0.0) ? albedoSample.a : 1.0;
                opacity = clamp(opacity, 0.0, 1.0);

                vec4 result = vec4(res, opacity);

                )";
            }

            if (customMainCode.at(type) != "")
            {
                // Any change that want to be made to the final pixel must be end up being applied to "result" and "opacity" variables
                shader << customMainCode.at(type) << "\n";
            }

            shader << R"(
            FragColor = result;
            //FragColor = vec4(texture(material.metallic, TexCoord).rgb, 1.0); 
            //FragColor = vec4(TexCoord, 0.0, 1.0);
            //FragColor = vec4(norm *0.5 + 0.5, 1.0);
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
            //TexCoord = mod(aTexCoord, 1.0);
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
        //std::cout << shader.str() << "\n";
        // ----------------- DEBUG -----------------

        return shader.str();
    }


    std::string ShaderLab::loadFromFile(const std::string& filePath)
    {
        std::ifstream file(filePath);

        if (!file.is_open()) throw std::runtime_error("No se pudo abrir el archivo: " + filePath);

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}