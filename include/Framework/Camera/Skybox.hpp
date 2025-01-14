#ifndef SKYBOX_H
#define SKYBOX_H

#include <Core/Graphics/Shader.hpp>
#include <Core/Graphics/TextureManager.hpp>
#include <Core/Graphics/Mesh.hpp>

namespace Essentia
{
    class Skybox
    {
        public:
            Skybox(const std::string& path, bool flip = false);
            Skybox(std::vector<std::string> faces, bool flip = false);
            void render(glm::mat4 projection, glm::mat4 view);

        private:
            Shader shader;
            std::shared_ptr<Essentia::Texture> texture;
            Mesh mesh;

            void setupMesh();
    };
}

#endif // !SKYBOX_H