#include <iostream>
#include <EssentiaEngine>

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



