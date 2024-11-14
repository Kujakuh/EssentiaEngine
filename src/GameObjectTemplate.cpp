#include <EssentiaEngine>

class GameObjectTemplate : EventListener
{
    public:

        GameObject entity;

        GameObjectTemplate(Essentia::Scene* scene) : entity(scene->CreateEntity("MyCustomGameObject"))
        {
            EventSystem<INTERNAL_EVENT>::addListener(this);
        }

        void onSysEvent(INTERNAL_EVENT event) override 
        {
            if (event == SYS_START) std::cout << "Sys Event Raised" << "\n";
        }

        void onEvent(int event) override {}
};