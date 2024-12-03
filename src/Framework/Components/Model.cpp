#include <Framework/Components/Model.hpp>

namespace Essentia
{
    Model::Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes) : meshes(initialMeshes) 
    { initializeShader(); }

    void Model::addMesh(const std::shared_ptr<Mesh>& mesh) { meshes.push_back(mesh); }

    size_t Model::getMeshCount() const { return meshes.size(); }
    const std::shared_ptr<Mesh>& Model::getMesh(size_t index) const
    {
        if (index >= meshes.size())
            throw std::out_of_range("Mesh index out of range");

        return meshes[index];
    }

    void Model::addCustomShaderFunction(SH_TYPE type, const std::string& functionCode)
    {
        shaderGenerator.addCustomFunction(type, functionCode);
        initializeShader();
    }

    void Model::addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode)
    {
        shaderGenerator.addMainCode(type, mainCode);
        initializeShader();
    }

    void Model::addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderGenerator.addCustomFunctionFromFile(type, filePath);
        initializeShader();
    }

    void Model::addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath)
    {
        shaderGenerator.addMainCodeFromFile(type, filePath);
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
        std::string vertexCode = shaderGenerator.generateShader3D(SH_TYPE::VERTEX);
        std::string fragmentCode = shaderGenerator.generateShader3D(SH_TYPE::FRAGMENT);

        if (shader) shader->recompileProgram(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA);
        else setShader(std::make_shared<Shader>(vertexCode.c_str(), fragmentCode.c_str(), DATA_SOURCE::STR_DATA));
    }

    void Model::loadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
            return;
        }
        dir = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }


    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            //meshes.push_back(std::make_shared<Mesh>(processMesh(mesh, scene)));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            processNode(node->mChildren[i], scene);
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        return Mesh();
    }
     
    std::vector<std::shared_ptr<Texture>> Model::loadMaterials(aiMaterial* mat, aiTextureType type, TEX_TYPE typeName)
    {
        std::vector<std::shared_ptr<Texture>> textures;
        for (size_t i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string file = dir + '/' + std::string(str.C_Str());

            std::shared_ptr<Texture> tx = TextureManager::getTexture(file, GL_TEXTURE_2D, typeName, Essentia::defaultFilters3D);
        }
        return textures;
    }
}