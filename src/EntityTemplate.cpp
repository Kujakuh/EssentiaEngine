#include <EssentiaEngine>
using namespace Essentia;

class EntityTemplate
{
public:

    GameObject entity;

    EntityTemplate(Essentia::Scene* scene)
    {
        entity = scene->GetEntityManager().CreateEntity("MyCustomEntity");
    }
    
};