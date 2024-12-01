#include <iostream>
#include <EssentiaEngine>

class SceneTemplate : public Scene
{
	public:
		SceneTemplate() {

		}

		void onInit() override
		{
			std::cout << "Scene Init" << '\n';
		}
		void onInit(const std::vector<Entity*>& entities, const std::vector<std::string>& arguments) override {}

		void onDestroy() override {}
		void onDestroy(std::vector<Entity*>& entities, std::vector<std::string>& arguments) override {}

		void onUpdate() override {}
		void RegisterSystems() override {}

		// DONT USE -- DEBUG ONLY, NOT A REAL FEATURE USAGE
		void RegisterSystems(Essentia::Renderer2D renderer)
		{
			systemDispatcher.RegisterSystem(std::make_shared<Essentia::Renderer2D>(renderer));
		}
};