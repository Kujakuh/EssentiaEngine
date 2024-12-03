#ifndef SHADER_GEN_H
#define SHADER_GEN_H

#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include <Shared/enums.hpp>
#include <Shared/user_values.hpp>

class ShaderGenerator
{
    private:
        std::string versionNextensionsHeader;
        std::unordered_map<SH_TYPE, std::string> shaderHeaders2D;
        std::unordered_map<SH_TYPE, std::string> shaderHeaders3D;
        std::unordered_map<SH_TYPE, std::vector<std::string>> customFunctions;
        std::unordered_map<SH_TYPE, std::string> customMainCode;
        std::vector<std::string> textureUniforms;

        std::string loadFromFile(const std::string& filePath);

    public:
        ShaderGenerator();

        void addTextureUniform(const std::string& textureFileName);
        void removeTextureUniform(const std::string& textureUniformName);

        void addCustomFunction(SH_TYPE type, const std::string& functionCode);
        void addCustomFunctionFromFile(SH_TYPE type, const std::string& filePath);

        void addMainCode(SH_TYPE type, const std::string& mainCode);
        void addMainCodeFromFile(SH_TYPE type, const std::string& filePath);

        std::string generateShader2D(SH_TYPE type) const;
        std::string generateShader3D(SH_TYPE type, bool ambientLightOn = true) const;
};

#endif // !SHADER_GEN_H