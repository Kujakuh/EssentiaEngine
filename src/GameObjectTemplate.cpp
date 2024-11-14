#include <EssentiaEngine>
using namespace Essentia;

class GameObjectTemplate : EventListener
{
    public:

        GameObject entity;

        GameObjectTemplate(Essentia::Scene* scene) : entity(scene->CreateEntity("MyCustomGameObject"))
        {
            EventSystem<INTERNAL_EVENT>::getInstance().addListener(this);
        }

        void onEvent(INTERNAL_EVENT event) override {
            if (event == A) std::cout << "TEST EVENT SYSTEM LISTENERS" << "\n";
        }
    
};