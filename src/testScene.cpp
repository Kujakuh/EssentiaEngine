#include <iostream>
#include <Core/ECS/ecs.hpp>

class testScene : public Essentia::Scene
{
	public:
		testScene() {

		}

		
	private:
		void onInit() override
		{
			std::cout << "Test Scene Init" << '\n';
		}
};



