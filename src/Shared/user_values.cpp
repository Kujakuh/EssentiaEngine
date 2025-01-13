#include <Shared/user_values.hpp>

namespace Essentia
{
	ska::flat_hash_map<FILTERS, GLenum> defaultFilters;
	ska::flat_hash_map<FILTERS, GLenum> defaultFilters3D;
	std::vector<Vertex> cubeVertices;
	std::vector<GLuint> cubeIndices;
    RENDER_MODE render_mode;
	bool bindlessTexturesMode;

    void init()
    {
		bindlessTexturesMode = true;
        render_mode = PBR;

        defaultFilters[FILTERS::MIN_F] = GL_NEAREST;
        defaultFilters[FILTERS::MAG_F] = GL_NEAREST;
        defaultFilters[FILTERS::WRAP_S] = GL_REPEAT;
        defaultFilters[FILTERS::WRAP_T] = GL_REPEAT;

		defaultFilters3D[FILTERS::MIN_F] = GL_LINEAR_MIPMAP_LINEAR;
		defaultFilters3D[FILTERS::MAG_F] = GL_LINEAR;
		defaultFilters3D[FILTERS::WRAP_S] = GL_REPEAT;
		defaultFilters3D[FILTERS::WRAP_T] = GL_REPEAT;

        cubeVertices = {
            // Front face (normales hacia el frente: +z)
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),

            // Back face (normales hacia atrás: -z)
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),

            // Left face (normales hacia la izquierda: -x)
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

            // Right face (normales hacia la derecha: +x)
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

            // Bottom face (normales hacia abajo: -y)
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

            // Top face (normales hacia arriba: +y)
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
            Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
            Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f))
        };

		cubeIndices =
		{
			// Front face
			0, 1, 2,  0, 2, 3,
			// Back face
			4, 5, 6,  4, 6, 7,
			// Left face
			8, 9, 10, 8, 10, 11,
			// Right face
			12, 13, 14, 12, 14, 15,
			// Bottom face
			16, 17, 18, 16, 18, 19,
			// Top face
			20, 21, 22, 20, 22, 23
		};
    }

    std::string generateUUID()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

        std::stringstream ss;

        ss << std::hex << std::setfill('0')
            << std::setw(8) << dist(gen) << '-'
            << std::setw(4) << (dist(gen) & 0xFFFF) << '-'
            << std::setw(4) << ((dist(gen) & 0x0FFF) | 0x4000) << '-'
            << std::setw(4) << ((dist(gen) & 0x3FFF) | 0x8000) << '-'
            << std::setw(12) << (dist(gen) & 0xFFFFFFFFFFFF);

        return ss.str();
    }
}
