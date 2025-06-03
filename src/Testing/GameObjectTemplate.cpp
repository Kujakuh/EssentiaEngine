#include <EssentiaEngine>

class GameObjectTemplate : EventListener
{
    public:
        GameObject entity;

        GameObjectTemplate(Essentia::Scene* scene) : entity(scene->CreateEntity("MyCustomGameObject"))
        {
            entity->onUpdate = [this]() {Update();};
			EventSystem::addListener(this);
        }

        void onEvent(const IEvent& event) override 
        {
			std::string type = event.getType();

            if (type == "InitEvent") {
				std::cout << event.getInfo() << "\n";
            }
        }
        void Update() {}
};
