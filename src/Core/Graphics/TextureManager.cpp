#include <Core/Graphics/TextureManager.hpp>

namespace Essentia
{
    int TextureManager::maxTextureUnits = 0;
    std::unordered_map<GLuint, std::set<int>> TextureManager::availableUnitsPerShader;
    std::unordered_map<GLuint, std::unordered_map<std::string, std::shared_ptr<Texture>>> TextureManager::textureCachePerShader;

    void TextureManager::detectMaxTextureUnits()
    {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    }

    void TextureManager::initializeForShader(GLuint shaderID)
    {
        if (maxTextureUnits == 0) detectMaxTextureUnits();

        if (availableUnitsPerShader.find(shaderID) == availableUnitsPerShader.end())
        {
            auto& units = availableUnitsPerShader[shaderID];
            for (int i = 0; i < maxTextureUnits; ++i)
            {
                units.insert(i);
            }
        }
    }

    int TextureManager::allocateUnit(GLuint shaderID)
    {
        initializeForShader(shaderID);

        auto& units = availableUnitsPerShader[shaderID];
        if (units.empty())
        {
            throw std::runtime_error("No available texture units for shader ID " + std::to_string(shaderID));
        }
        int unit = *units.begin();
        units.erase(units.begin());
        return unit;
    }

    std::shared_ptr<Texture> TextureManager::getTexture(const std::string& texturePath, GLenum textureType,
        TEX_TYPE type, const ska::flat_hash_map<FILTERS, GLenum>& filters, bool flip)
    {
        GLint shaderID = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shaderID);

        initializeForShader(shaderID);

        auto& shaderCache = textureCachePerShader[shaderID];

        auto it = shaderCache.find(texturePath);
        if (it != shaderCache.end())
        {
            return it->second;
        }

        // Cargar una nueva textura
        std::shared_ptr<Texture> texture;
        if (!glfwExtensionSupported("GL_ARB_bindless_texture"))
        {
            int unit = allocateUnit(shaderID);
            texture = std::make_shared<Texture>(texturePath.c_str(), textureType, unit, filters, type, flip);
            shaderCache[texturePath] = texture;
        }
        else
        {
            texture = std::make_shared<Texture>(texturePath.c_str(), textureType, 0, filters, type, flip);
            shaderCache[texturePath] = texture;
        }

        return texture;
    }

    std::shared_ptr<Texture> TextureManager::getCubemapTexture(const std::vector<std::string>& faces, GLenum textureType,
        TEX_TYPE type, const ska::flat_hash_map<FILTERS, GLenum>& filters)
    {
        GLint shaderID = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shaderID);

        initializeForShader(shaderID);

        std::string key = generateCubemapKey(faces);
        auto& shaderCache = textureCachePerShader[shaderID];

        auto it = shaderCache.find(key);
        if (it != shaderCache.end())
        {
            return it->second;
        }

        // Cargar un nuevo cubemap
        int unit = allocateUnit(shaderID);
        auto texture = std::make_shared<Texture>(faces, textureType, unit, filters, type);
        shaderCache[key] = texture;

        return texture;
    }

    void TextureManager::clearCache()
    {
        for (auto& [shaderID, shaderCache] : textureCachePerShader)
        {
            auto& units = availableUnitsPerShader[shaderID];

            for (const auto& [path, texture] : shaderCache)
            {
                units.insert(texture->getTextureUnit());
            }

            shaderCache.clear();
        }
    }

    void TextureManager::releaseUnit(int unit)
    {
        for (auto& [shaderID, units] : availableUnitsPerShader)
        {
            units.insert(unit);
        }
    }

    std::string TextureManager::getTexturePath(const std::shared_ptr<Texture>& texture)
    {
        for (const auto& [shaderID, shaderCache] : textureCachePerShader)
        {
            for (const auto& [path, tex] : shaderCache)
            {
                if (tex == texture) return path;
            }
        }
        return {};
    }

    std::string TextureManager::generateCubemapKey(const std::vector<std::string>& faces)
    {
        std::string key = "cubemap:";
        for (const auto& face : faces)
            key += face + ";";

        return key;
    }
}
