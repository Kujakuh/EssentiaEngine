#include <iostream>
#include <EssentiaEngine>
using namespace Essentia;

class SceneTemplate : public Scene
{
	public:
		SceneTemplate() {

		}
		
	private:
		void onInit() override
		{
			std::cout << "Scene Init" << '\n';
		}

};



