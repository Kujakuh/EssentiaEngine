#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Core/ECS/IComponent.hpp>
#include <Core/Graphics/Mesh.hpp>
#include <Core/Graphics/Texture.hpp>
#include <Core/Graphics/TextureManager.hpp>
#include <Core/Graphics/ShaderGen.hpp>
#include <Shared/enums.hpp>

namespace Essentia
{
    struct Model : public IComponent
    {
        private:
            std::string dir;
            std::shared_ptr<Shader> shader;
            ShaderGenerator shaderGenerator;

            void initializeShader();

            void processNode(aiNode* node, const aiScene* scene);
            //Mesh processMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<std::shared_ptr<Texture>> loadMaterials(aiMaterial* mat, aiTextureType type, TEX_TYPE typeName);

        public:
            std::vector<std::shared_ptr<Mesh>> meshes;

            Model() = default;
            Model(const std::vector<std::shared_ptr<Mesh>>& initialMeshes);

            void addMesh(const std::shared_ptr<Mesh>& mesh);

            size_t getMeshCount() const;
            const std::shared_ptr<Mesh>& getMesh(size_t index) const;

            void loadModel(const std::string& path);

            void addCustomShaderFunction(SH_TYPE type, const std::string& functionCode);
            void addCustomShaderMainCode(SH_TYPE type, const std::string& mainCode);
            void addCustomShaderFunctionFromFile(SH_TYPE type, const std::string& filePath);
            void addCustomShaderMainCodeFromFile(SH_TYPE type, const std::string& filePath);

            void setShader(std::shared_ptr<Shader> _shader);
            std::shared_ptr<Shader> getShader() const;
    };
}

#endif // !MODEL_H