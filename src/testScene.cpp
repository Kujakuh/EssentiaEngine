#include <iostream>
#include <Core/Essentia_ECS>

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



