#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <EssentiaEngine>

class Player : public EventListener
{
    public:
        GameObject entity;

        Player(Essentia::Scene* scene);
        void onEvent(const IEvent& event) override;
        void Update();
};

#endif // PLAYER_HPP