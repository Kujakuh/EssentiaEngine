#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <flat_hash_map>
#include <json.hpp>
#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <cmath>

#include <Shared/enums.hpp>
#include <Shared/user_values.hpp>
#include <Core/Graphics/UVRegion.hpp>

namespace Essentia
{
    class Texture
    {
        private:
            GLuint ID = 0;
            GLenum type;
            int textureUnit;
            ska::flat_hash_map<FILTERS, GLenum> wrapFilters;
            int width = 0, height = 0, nrChannels = 0;

            float* hdriData;

            TEX_TYPE texType;

            bool bindlessSupported = false;
            GLuint64 textureHandle = 0;

            std::unordered_map<std::string, UVRegion> uvRegions;

            void flipVertically(unsigned char* data, int width, int height, int nrChannels);

        public:
            Texture(const std::vector<std::string>& faces, GLenum textureType, int textureUnit,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip = true);

            Texture(const char* texturePath, GLenum textureType, int textureUnit,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip = true);

            ~Texture();

            int hdriRes = 0;

            std::string getTextureTypeString() const;

            GLuint getID() const;
            GLuint64 getHandle() const;
            bool isBindless() const;
            bool hasAlpha() const;

            void enableBindless();

            int getTextureUnit() const;
            GLenum getFilter(FILTERS key) const;
            int getWidth() const;
            int getHeight() const;

            bool filterExists(FILTERS key) const;

            bool isAtlasTexture() const;
            void addUVRegion(const std::string& name, const UVRegion& region);
            UVRegion getUVRegion(const std::string& name) const;
            void removeUVRegion(const std::string& name);
            void removeAllUVRegions();
            void updateUVRegion(const std::string& name, const UVRegion& newRegion);
            void loadUVsFromJSON(const std::string& filePath);

            void modifyFilters(const ska::flat_hash_map<FILTERS, GLenum>& newFilters);
            void modifyFilter(FILTERS key, GLenum value);
            void applyFilters();

            void bind() const;
            void unbind() const;

            void loadFromFile(const char* texturePath, bool flip = false);
            //void loadEXR(const char* texturePath, bool flip = true);
            void loadCubemap(const std::vector<std::string>& faces, bool flip);
            void loadHDRIToCubemap(const std::string& hdriPath, unsigned int cubemapResolution = 1024, bool linearFilter = true, bool flip = true);
    };
}

#endif // !TEXTURE_H