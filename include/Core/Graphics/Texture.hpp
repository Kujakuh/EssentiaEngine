#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <flat_hash_map>
#include <string>
#include <iostream>

#include <Core/enums.hpp>

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

            TEX_TYPE texType;

        public:
            Texture(const std::vector<std::string>& faces, GLenum textureType, int textureUnit,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type);

            Texture(const char* texturePath, GLenum textureType, int textureUnit,
                const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip = true);


            ~Texture();

            std::string getTextureTypeString() const;
            GLuint getID() const;
            int getTextureUnit() const;
            GLenum getFilter(FILTERS key) const;

            bool filterExists(FILTERS key) const;

            void modifyFilters(const ska::flat_hash_map<FILTERS, GLenum>& newFilters);
            void modifyFilter(FILTERS key, GLenum value);
            void applyFilters();

            void bind() const;
            void unbind() const;

            void loadFromFile(const char* texturePath, bool flip = true);
            void loadCubemap(const std::vector<std::string>& faces);
    };
}

#endif // !TEXTURE_H