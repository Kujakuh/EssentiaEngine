#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/HdriCubemap/hdriCubemap.hpp>

namespace Essentia
{
    Texture::Texture(const char* texturePath, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip)
        : type(textureType), textureUnit(textureUnit), wrapFilters(filters), texType(type)
    {
        if (type == TEX_TYPE::TEX_CUBEMAP)
            if (hdriRes != 0) loadHDRIToCubemap(texturePath, hdriRes);
            else loadHDRIToCubemap(texturePath);
        else
            loadFromFile(texturePath, flip);

        bindlessSupported = GLAD_GL_ARB_bindless_texture && bindlessTexturesMode;
        if (bindlessSupported) { enableBindless(); }
    }

    Texture::Texture(const std::vector<std::string>& faces, GLenum textureType, int textureUnit,
        const ska::flat_hash_map<FILTERS, GLenum>& filters, TEX_TYPE type, bool flip)
        : type(textureType), textureUnit(textureUnit), wrapFilters(filters), texType(type)
    {
        if (type != TEX_TYPE::TEX_CUBEMAP)
            throw std::invalid_argument("Invalid constructor for non-cubemap texture.");
        loadCubemap(faces, flip);

        bindlessSupported = GLAD_GL_ARB_bindless_texture && bindlessTexturesMode;
        if (bindlessSupported) { enableBindless(); }
    }

    Texture::~Texture()
    {
        if (bindlessSupported && textureHandle != 0) {
            glMakeTextureHandleNonResidentARB(textureHandle);
        }
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
    }

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
    GLuint64 Texture::getHandle() const { return textureHandle; }
    bool Texture::isBindless() const { return bindlessSupported; }
    bool Texture::hasAlpha() const { return nrChannels == 4; }

    void Texture::enableBindless()
    {
        textureHandle = glGetTextureHandleARB(this->ID);
        if (textureHandle) {
            glMakeTextureHandleResidentARB(textureHandle);
        }
        else {
            std::cerr << "ERROR::TEXTURE::BINDLESS_TEXTURE::FAILED_TO_CREATE_HANDLE" << std::endl;
        }
    }

    int Texture::getTextureUnit() const { return textureUnit; }
    bool Texture::filterExists(FILTERS key) const {return wrapFilters.find(key) != wrapFilters.end();}
    GLenum Texture::getFilter(FILTERS key) const
    {
        auto it = wrapFilters.find(key);
        if (it != wrapFilters.end()) return it->second;
        std::cerr << "ERROR::TEXTURE::FILTER_KEY_NOT_FOUND" << std::endl;
        return 0;
    }
    int Texture::getWidth() const { return width; }
    int Texture::getHeight() const { return height; }

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
        if (!bindlessSupported)
            glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(texType == TEX_TYPE::TEX_CUBEMAP ? GL_TEXTURE_CUBE_MAP : type, ID);
    }

    void Texture::unbind() const
    {
        if (!bindlessSupported) 
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

        // Detecta la extensión del archivo
        std::string pathStr = texturePath;
        std::string extension = pathStr.substr(pathStr.find_last_of('.') + 1);
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        if (extension == "exr")
        {
            loadEXR(texturePath);
            return;
        }

        // Carga estándar con stb_image
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

    void Texture::loadEXR(const char* texturePath, bool flip)
    {
        try
        {
            Imf::Array2D<Imf::Rgba> pixels;
            int exrWidth, exrHeight;

            // Lee el archivo EXR
            Imf::RgbaInputFile file(texturePath);
            Imath::Box2i dw = file.dataWindow();
            exrWidth = dw.max.x - dw.min.x + 1;
            exrHeight = dw.max.y - dw.min.y + 1;

            // Reserva memoria para los píxeles
            pixels.resizeErase(exrHeight, exrWidth);

            // Lee los píxeles al buffer
            file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * exrWidth, 1, exrWidth);
            file.readPixels(dw.min.y, dw.max.y);

            // Convierte a un formato adecuado para OpenGL
            std::vector<float> texData(exrWidth * exrHeight * 4); // RGBA
            for (int y = 0; y < exrHeight; ++y)
            {
                for (int x = 0; x < exrWidth; ++x)
                {
                    int srcY = flip ? (exrHeight - 1 - y) : y;
                    const Imf::Rgba& px = pixels[srcY][x];
                    int idx = (y * exrWidth + x) * 4;
                    texData[idx + 0] = px.r;
                    texData[idx + 1] = px.g;
                    texData[idx + 2] = px.b;
                    texData[idx + 3] = px.a;
                }
            }

            // Subir textura a OpenGL
            glGenTextures(1, &ID);
            bind();

            glTexImage2D(type, 0, GL_RGBA32F, exrWidth, exrHeight, 0, GL_RGBA, GL_FLOAT, texData.data());
            glGenerateMipmap(type);
            applyFilters();

            unbind();

            std::cout << "Successfully loaded EXR texture: " << texturePath << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "ERROR::EXR::FAILED_TO_LOAD " << texturePath << ": " << e.what() << std::endl;
        }
    }


    void Texture::loadCubemap(const std::vector<std::string>& faces, bool flip)
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
            if (flip && i != 1 && i != 3)
                stbi_set_flip_vertically_on_load(true);
            else
                stbi_set_flip_vertically_on_load(false);

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

    void Texture::loadHDRIToCubemap(const std::string& hdriPath, unsigned int cubemapResolution, bool linearFilter, bool flip) {

        if (ID != 0) 
        {
            glDeleteTextures(1, &ID);
            ID = 0;
        }
        try 
        {
            HdriToCubemap<unsigned char> cubemap(hdriPath, cubemapResolution, linearFilter);

            this->width = cubemap.getCubemapResolution();
            this->height = cubemap.getCubemapResolution();
            this->nrChannels = cubemap.getNumChannels();

            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

            std::vector<unsigned char*> data = {
                cubemap.getRight(),
                cubemap.getLeft(),
                cubemap.getUp(),
                cubemap.getDown(),
                cubemap.getFront(),
                cubemap.getBack()
            };

            for (int i = 0; i < 6; ++i) 
            {
                flipVertically(data[i], cubemapResolution, cubemapResolution, nrChannels);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cubemapResolution, cubemapResolution, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
            }

            applyFilters();
            glBindTexture(type, 0);
        }
        catch (const std::exception& e) { std::cerr << "ERROR::TEXTURE::HDRI_TO_CUBEMAP::" << e.what() << std::endl; }
    }

    void Texture::flipVertically(unsigned char* data, int width, int height, int nrChannels) {
        int rowSize = width * nrChannels;
        std::vector<unsigned char> rowBuffer(rowSize);

        for (int y = 0; y < height / 2; ++y) {
            unsigned char* topRow = data + y * rowSize;
            unsigned char* bottomRow = data + (height - y - 1) * rowSize;

            // Intercambia las filas
            std::copy(bottomRow, bottomRow + rowSize, rowBuffer.data());
            std::copy(topRow, topRow + rowSize, bottomRow);
            std::copy(rowBuffer.data(), rowBuffer.data() + rowSize, topRow);
        }
    }

}