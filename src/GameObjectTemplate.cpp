#include <EssentiaEngine>
using namespace Essentia;

class GameObjectTemplate
{
    public:

        GameObject entity;

        GameObjectTemplate(Essentia::Scene* scene) : entity(scene->CreateEntity("MyCustomGameObject"))
        {

        }
    
};