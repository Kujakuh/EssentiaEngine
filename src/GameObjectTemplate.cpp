#include <EssentiaEngine>
using namespace Essentia;

class GameObjectTemplate : EventListener
{
    public:

        GameObject entity;

        GameObjectTemplate(Essentia::Scene* scene) : entity(scene->CreateEntity("MyCustomGameObject"))
        {
            EventSystem<INTERNAL_EVENT>::addListener(this);
        }

        void onEvent(INTERNAL_EVENT event) override {
            if (event == SYS_START) std::cout << "TEST EVENT SYSTEM LISTENERS" << "\n";
        }
    
};