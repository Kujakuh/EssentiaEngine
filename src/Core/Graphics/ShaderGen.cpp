#include <Core/Graphics/ShaderGen.hpp>

ShaderGenerator::ShaderGenerator()
{
    shaderHeaders2D[VERTEX] = "";
    shaderHeaders2D[FRAGMENT] = "";
    shaderHeaders2D[GEOMETRY] = "";

    customFunctions[VERTEX] = {};
    customFunctions[FRAGMENT] = {};
    customFunctions[GEOMETRY] = {};

    customMainCode[VERTEX] = "";
    customMainCode[FRAGMENT] = "";
    customMainCode[GEOMETRY] = "";

    // Header para vertex shader (simple 2D)
    shaderHeaders2D[VERTEX] = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 2) in vec2 aTexCoord;

    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
    )";

    // Header para fragment shader (base)
    shaderHeaders2D[FRAGMENT] = R"(
    #version 330 core
    in vec2 TexCoord;

    out vec4 FragColor;
    )";

    // Header para geometry shader (si se necesita)
    shaderHeaders2D[GEOMETRY] = R"(
    #version 330 core
    layout(points) in;
    layout(triangle_strip, max_vertices = 4) out;

    void main() {
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
void ShaderGenerator::addMainCode(SH_TYPE type, const std::string& mainCode) {
    customMainCode[type] += mainCode + "\n";
}
void ShaderGenerator::addMainCodeFromFile(SH_TYPE type, const std::string& filePath)
{
    customMainCode[type] += loadFromFile(filePath) + "\n";
}

std::string ShaderGenerator::generateShader2D(SH_TYPE type) const
{
    std::ostringstream shader;

    shader << shaderHeaders2D.at(type) << "\n";

    // Add Texture Uniforms declarations
    if (type == FRAGMENT) 
    {
        for (const auto& uniform : textureUniforms)
            shader << "uniform sampler2D "  << uniform << ";\n";
    }

    // Add pre-main content (custom functions and others)
    if (customFunctions.at(type).empty())
    {
        if (type == FRAGMENT)
            shader << R"(
            vec4 defaultBlend(sampler2D tex, vec2 uv) {
                return texture(tex, uv);
            }
            )";
    }
    else 
    {
        for (const auto& function : customFunctions.at(type))
            shader << function << "\n";
    }

    // Generate main
    if (type != VERTEX)
    {
        shader << R"(
        void main() {
        )";

        if (customMainCode.at(type).empty()) {
            if (type == FRAGMENT && !textureUniforms.empty())
            {
                shader << "    FragColor = texture(" << textureUniforms[0] << ", TexCoord);\n";
            }
            else if (type == FRAGMENT)
            {
                shader << "    FragColor = vec4(1.0);\n";
            }
        }
        else shader << customMainCode.at(type); // Add any custom main code

        shader << R"(
        }
        )";
    }

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