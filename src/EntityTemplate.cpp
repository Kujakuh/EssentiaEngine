#include <EssentiaEngine>
using namespace Essentia;

class EntityTemplate
{
public:

    Entity* entity;

    EntityTemplate(Essentia::Scene* scene)
    {
        entity = &(scene->GetEntityManager().CreateEntity("MyCustomEntity"));
    }
    
};