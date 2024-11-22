#include <Core/Graphics/Texture.hpp>

namespace Essentia
{
    Texture::Texture(const char* texturePath, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip)
        : type(textureType), textureUnit(textureUnit), wrapFilters(filters), texType(type)
    {
        if (type == TEX_TYPE::TEX_CUBEMAP)
            throw std::invalid_argument("Invalid constructor for cubemap texture.");
        loadFromFile(texturePath, flip);
    }

    Texture::Texture(const std::vector<std::string>& faces, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type)
        : type(textureType), textureUnit(textureUnit), wrapFilters(filters), texType(type)
    {
        if (type != TEX_TYPE::TEX_CUBEMAP)
            throw std::invalid_argument("Invalid constructor for non-cubemap texture.");
        loadCubemap(faces);
    }

    Texture::~Texture() {if (ID != 0) glDeleteTextures(1, &ID);}

    std::string Texture::getTextureTypeString() const
    {
        switch (texType)
        {
            case TEX_TYPE::TEX_DIFF: return "diffuse";
            case TEX_TYPE::TEX_SPEC: return "specular";
            case TEX_TYPE::TEX_NORM: return "normal";
            case TEX_TYPE::TEX_HEIGHT: return "height";
            case TEX_TYPE::TEX_CUBEMAP: return "cubemap";
            default: return "Undefined";
        }
    }

    GLuint Texture::getID() const { return ID; }
    int Texture::getTextureUnit() const { return textureUnit; }
    bool Texture::filterExists(FILTERS key) const {return wrapFilters.find(key) != wrapFilters.end();}
    GLenum Texture::getFilter(FILTERS key) const
    {
        auto it = wrapFilters.find(key);
        if (it != wrapFilters.end()) return it->second;
        std::cerr << "ERROR::TEXTURE::FILTER_KEY_NOT_FOUND" << std::endl;
        return 0;
    }

    void Texture::modifyFilters(const ska::flat_hash_map<FILTERS, GLenum>& newFilters)
    {
        wrapFilters = newFilters;
        applyFilters();
    }

    void Texture::modifyFilter(FILTERS key, GLenum value)
    {
        wrapFilters[key] = value;
        applyFilters();
    }

    void Texture::applyFilters()
    {
        bind();

        if (texType == TEX_TYPE::TEX_CUBEMAP)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                filterExists(FILTERS::MIN_F) ? wrapFilters.at(FILTERS::MIN_F) : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                filterExists(FILTERS::MAG_F) ? wrapFilters.at(FILTERS::MAG_F) : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                filterExists(FILTERS::WRAP_S) ? wrapFilters.at(FILTERS::WRAP_S) : GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                filterExists(FILTERS::WRAP_T) ? wrapFilters.at(FILTERS::WRAP_T) : GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                filterExists(FILTERS::WRAP_R) ? wrapFilters.at(FILTERS::WRAP_R) : GL_CLAMP_TO_EDGE);
        }
        else
        {
            if (filterExists(FILTERS::WRAP_S))
                glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapFilters.at(FILTERS::WRAP_S));
            if (filterExists(FILTERS::WRAP_T))
                glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapFilters.at(FILTERS::WRAP_T));
            if (filterExists(FILTERS::MIN_F))
                glTexParameteri(type, GL_TEXTURE_MIN_FILTER, wrapFilters.at(FILTERS::MIN_F));
            if (filterExists(FILTERS::MAG_F))
                glTexParameteri(type, GL_TEXTURE_MAG_FILTER, wrapFilters.at(FILTERS::MAG_F));
        }
        unbind();
    }

    void Texture::bind() const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(texType == TEX_TYPE::TEX_CUBEMAP ? GL_TEXTURE_CUBE_MAP : type, ID);
    }

    void Texture::unbind() const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(texType == TEX_TYPE::TEX_CUBEMAP ? GL_TEXTURE_CUBE_MAP : type, 0);
    }

    void Texture::loadFromFile(const char* texturePath, bool flip)
    {
        if (ID != 0)
        {
            glDeleteTextures(1, &ID);
            ID = 0;
        }
        if (flip) stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (!data)
        {
            std::cerr << "ERROR::TEXTURE::FAILED_TO_LOAD " << texturePath << std::endl;
            return;
        }

        glGenTextures(1, &ID);
        bind();

        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(type);
        applyFilters();

        stbi_image_free(data);
        unbind();
    }

    void Texture::loadCubemap(const std::vector<std::string>& faces)
    {
        if (ID != 0)
        {
            glDeleteTextures(1, &ID);
            ID = 0;
        }

        glGenTextures(1, &ID);
        glBindTexture(type, ID);

        for (size_t i = 0; i < faces.size(); ++i)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cerr << "ERROR::TEXTURE::CUBEMAP::FAILED_TO_LOAD " << faces[i] << std::endl;
                stbi_image_free(data);
                glBindTexture(type, 0);
                return;
            }
        }

        applyFilters();
        glBindTexture(type, 0);
    }
}