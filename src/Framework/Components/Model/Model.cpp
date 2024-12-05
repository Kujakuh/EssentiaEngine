#include <Framework/Components/Model/Model.hpp>
#include <Framework/Components/Model/ModelCacheManager.hpp>

namespace Essentia
{
    Model::Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes) : meshes(initialMeshes) 
    { initializeShader(); }

    Model::Model(const std::string& path, bool inverseUvY)
    { 
        loadModel(path, inverseUvY);
    }

    void Model::loadModel(const std::string& path, bool inverseUvY)
    {
        *this = *(ModelCacheManager::getInstance().loadModel(path, inverseUvY));
    }

    void Model::addMesh(const std::shared_ptr<Mesh>& mesh) { meshes.push_back(mesh); }

    size_t Model::getMeshCount() const { return meshes.size(); }
    const std::shared_ptr<Mesh>& Model::getMesh(size_t index) const
    {
        if (index >= meshes.size())
            throw std::out_of_range("Mesh index out of range");

        return meshes[index];
    }
    bool Model::hasAlpha() const
    {
        for (auto& mesh : meshes)
            if (mesh->materials.size() > 0) if (mesh->materials[0].hasAlpha()) return true;

        return false;
    }

    void Model::addCustomShaderFunction(SH_TYPE type, const std::string& functionCode)
    {
        shaderLab.addCustomFunction(type, functionCode);
        initializeShader();
    }

    void Model::addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode)
    {
        shaderLab.addMainCode(type, mainCode);
        initializeShader();
    }

    void Model::addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderLab.addCustomFunctionFromFile(type, filePath);
        initializeShader();
    }

    void Model::addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderLab.addMainCodeFromFile(type, filePath);
        initializeShader();
    }

    void Model::setShader(std::shared_ptr<Shader> _shader)
    {
        shader = _shader;
        for (auto& mesh : meshes) mesh->shader = shader;
    }
    std::shared_ptr<Shader> Model::getShader() const { return shader; }

    void Model::initializeShader()
    {
        std::string vertexCode = shaderLab.generateShader3D(SH_TYPE::VERTEX);
        std::string fragmentCode = shaderLab.generateShader3D(SH_TYPE::FRAGMENT);

        if (shader) shader->recompileProgram(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA);
        else setShader(std::make_shared<Shader>(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA));
    }

    void Model::loadModelInner(const std::string& path, bool inverseUvY)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
            return;
        }
        dir = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene, inverseUvY);
        if(shader) shader.reset();
    }


    void Model::processNode(aiNode* node, const aiScene* scene, bool inverseUvY)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(std::make_shared<Mesh>(processMesh(mesh, scene, inverseUvY)));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            processNode(node->mChildren[i], scene, inverseUvY);
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, bool inverseUvY)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Material> materials;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            glm::vec3 normal(0.0f);
            glm::vec2 texCoords(0.0f);
            glm::vec3 tangent(1.0f), bitangent(0.0f);

            if (mesh->HasNormals()) {
                normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }

            if (mesh->mTextureCoords[0]) {
                if(inverseUvY) texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y);
                else texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            }

            Vertex vertex(position, normal, texCoords, tangent, bitangent);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        materials = loadMaterials(material);

        return Mesh(shader, vertices, indices, materials);
    }
    
    std::vector<Essentia::Material> Model::loadMaterials(aiMaterial* mat) {
        std::vector<Essentia::Material> materials;

        struct TextureType {
            aiTextureType aiType;
            TEX_TYPE texType;
            std::function<void(Essentia::Material&, std::shared_ptr<Texture>)> setter;
        };

        std::vector<TextureType> textureTypes = {
            { aiTextureType_DIFFUSE, TEX_TYPE::TEX_DIFF, [](Essentia::Material& mat, std::shared_ptr<Texture> tex) { mat.diffuse = tex; } },
            { aiTextureType_SPECULAR, TEX_TYPE::TEX_SPEC, [](Essentia::Material& mat, std::shared_ptr<Texture> tex) { mat.specular = tex; } },
            { aiTextureType_NORMALS, TEX_TYPE::TEX_NORM, [](Essentia::Material& mat, std::shared_ptr<Texture> tex) { mat.normal = tex; } },
            { aiTextureType_HEIGHT, TEX_TYPE::TEX_HEIGHT, [](Essentia::Material& mat, std::shared_ptr<Texture> tex) { mat.height = tex; } },
            { aiTextureType_OPACITY, TEX_TYPE::TEX_OPA, [](Essentia::Material& mat, std::shared_ptr<Texture> tex) { mat.alpha = tex; } }
        };

        size_t maxTextures = 0;
        for (const auto& textureType : textureTypes) {
            maxTextures = std::max(maxTextures, static_cast<size_t>(mat->GetTextureCount(textureType.aiType)));
        }

        materials.resize(maxTextures);

        for (const auto& textureType : textureTypes) {
            auto textures = loadMaterialsTextures(mat, textureType.aiType, textureType.texType);
            for (size_t i = 0; i < textures.size(); ++i) {
                textureType.setter(materials[i], textures[i]);
            }
        }
        return materials;
    }

    std::vector<std::shared_ptr<Texture>> Model::loadMaterialsTextures(aiMaterial* mat, aiTextureType type, TEX_TYPE typeName)
    {
        std::vector<std::shared_ptr<Texture>> textures;
        for (uint8_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string file = dir + '/' + std::string(str.C_Str());

            std::shared_ptr<Texture> tx = TextureManager::getTexture(file, GL_TEXTURE_2D, typeName, Essentia::defaultFilters3D);
            if (tx) { textures.push_back(tx); }
        }
        return textures;
    }
}