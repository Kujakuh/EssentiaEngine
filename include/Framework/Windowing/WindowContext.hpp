#ifndef WINDOW_CONTEXT_H
#define WINDOW_CONTEXT_H

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>

#include <Core/ECS/Scene.hpp>

namespace Essentia
{
	struct WindowContext
	{
		GLFWwindow* window;
		int windowId;
		std::vector<Scene*> updatedScenes;
		Scene* activeScene = nullptr;
	};
}

#endif // !WINDOW_CONTEXT_H