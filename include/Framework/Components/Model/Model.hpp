#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/TextureManager.hpp>
#include <Core/Graphics/ShaderLab.hpp>
#include <Shared/enums.hpp>

namespace Essentia
{
    struct Model : public IComponent
    {
        private:
            std::string dir;
            std::shared_ptr<Shader> shader;

            void initializeShader();

            void processNode(aiNode* node, const aiScene* scene, bool inverseUvY);
            Mesh processMesh(aiMesh* mesh, const aiScene* scene, bool inverseUvY);
            std::vector<Essentia::Material> loadMaterials(aiMaterial* mat);
            std::vector<std::shared_ptr<Texture>> loadMaterialsTextures(aiMaterial* mat, aiTextureType type, TEX_TYPE typeName);
            //std::vector<std::shared_ptr<Texture>> loadMaterialsTexturesAsync(aiMaterial* mat, aiTextureType type, TEX_TYPE typeName);

            void loadModelInner(const std::string& path, bool inverseUvY = true);

        public:
            std::vector<std::shared_ptr<Mesh>> meshes;
            ShaderLab shaderLab;

            Model() = default;
            Model(const Mesh mesh);
            Model(const std::string& path, bool inverseUvY = true);
            Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes);

            void loadModel(const std::string& path, bool inverseUvY = true);

            void addMesh(const std::shared_ptr<Mesh>& mesh);

            size_t getMeshCount() const;
            const std::shared_ptr<Mesh>& getMesh(size_t index) const;
            bool hasAlpha() const;

            void addCustomShaderFunction(SH_TYPE type, const std::string& functionCode);
            void addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode);
            void addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath);
            void addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath);

            void setShader(std::shared_ptr<Shader> _shader);
            std::shared_ptr<Shader> getShader() const;

            friend class ModelCacheManager;
    };
}

#endif // !MODEL_H