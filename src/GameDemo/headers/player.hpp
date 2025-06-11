#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <EssentiaEngine>

enum Direccion {
    left, right, up, down, idle
};

class Player : public EventListener
{
    public:
        GameObject entity;

        Player(Essentia::Scene* scene);
        void onEvent(const IEvent& event) override;
        void Update();

    private:
	    void setUpAnimations();

	    Sprite* sprite = nullptr;
	    Animator* animator = nullptr;
		Transform* transform = nullptr;
		Direccion direction = down;
        float speed = 2.5f;
};

#endif // PLAYER_HPP