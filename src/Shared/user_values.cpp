#include <Shared/user_values.hpp>

namespace Essentia
{
    ska::flat_hash_map<FILTERS, GLenum> defaultFilters;
	std::vector<Vertex> cubeVertices;
	std::vector<GLuint> cubeIndices;

    void init()
    {
        defaultFilters[FILTERS::MIN_F] = GL_LINEAR;
        defaultFilters[FILTERS::MAG_F] = GL_LINEAR;
        defaultFilters[FILTERS::WRAP_S] = GL_REPEAT;
        defaultFilters[FILTERS::WRAP_T] = GL_REPEAT;

		cubeVertices =
		{
			// Front face
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f)),

			// Back face
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),

			// Left face
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),

			// Right face
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f)),

			// Bottom face
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f)),

			// Top face
			Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f))
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
}
