#include <iostream>
#include <EssentiaEngine>

class SceneTemplate : public Scene
{
	public:

		/*
		*	Scene resources declaration, EG:
		* 
		*	CameraPerspective *cam;
		*	Renderer3D rend3d;
		*
		*/

		SceneTemplate() 
		{
			//	Scene resources initialitation, EG:
			// 
			//	cam = &CameraPerspective("Camera", this, 45.0f, (float)1920 / (float)1080, 0.1f, 100.0f);
			//	rend3d = Renderer3D(cam);
		}

		void onInit() override
		{
			std::cout << "Scene Init" << '\n';
		}
		void onInit(const std::vector<Entity*>& entities, const std::vector<std::string>& arguments) override {}

		void onDestroy() override {}
		void onDestroy(std::vector<Entity*>& entities, std::vector<std::string>& arguments) override {}

		void onUpdate() override {}
		void RegisterSystems() override 
		{
			//	Scene Systems register
			// 
			//	systemDispatcher.RegisterSystem(std::make_shared<Essentia::Renderer3D>(rend3d));
		}

		// DONT USE -- DEBUG ONLY, NOT A REAL FEATURE USAGE
		void RegisterSystems(Essentia::Renderer2D renderer, Essentia::Renderer3D renderer3d)
		{
			systemDispatcher.RegisterSystem(std::make_shared<Essentia::Renderer2D>(renderer));
			systemDispatcher.RegisterSystem(std::make_shared<Essentia::Renderer3D>(renderer3d));
		}
};