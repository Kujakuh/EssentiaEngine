#include <Framework/Components/Model.hpp>

namespace Essentia
{
    Model::Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes): meshes(initialMeshes) {}

    void Model::addMesh(const std::shared_ptr<Mesh>& mesh) { meshes.push_back(mesh); }

    size_t Model::getMeshCount() const { return meshes.size(); }
    const std::shared_ptr<Mesh>& Model::getMesh(size_t index) const
    {
        if (index >= meshes.size())
            throw std::out_of_range("Mesh index out of range");

        return meshes[index];
    }

    std::vector<std::string> Model::getTexturePaths() const
    {
        std::vector<std::string> paths;

        for (const auto& mesh : meshes)
        {
            for (const auto& texturePair : mesh->textures)
            {
                if (texturePair.second) 
                    paths.push_back(TextureManager::getTexturePath(texturePair.second));
            }
        }
        return paths;
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
            meshes.push_back(std::make_shared<Mesh>(processMesh(mesh, scene)));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
            processNode(node->mChildren[i], scene);
    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
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
    }
}