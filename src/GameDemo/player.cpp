#include "headers/player.hpp"

Player::Player(Essentia::Scene* scene) : entity(scene->CreateEntity("Player_main"))
{
    entity->onUpdate = [this]() { Update(); };
    EventSystem::addListener(this);

    entity->AddComponent<Sprite>(RESOURCES_PATH "GameDemo/link.png");
    entity->AddComponent<Animator>();

    sprite = entity->GetComponent<Sprite>();
    animator = entity->GetComponent<Animator>();
	transform = entity->GetComponent<Transform>();

    sprite->getTexture()->loadUVsFromJSON(RESOURCES_PATH "GameDemo/link.json");
	transform->setScale(Vector3(15.0f));

	setUpAnimations();
}

void Player::onEvent(const IEvent& event)
{
    std::string type = event.getType();
    if (type == "InitEvent") {
        std::cout << event.getInfo() << "\n";
    }
}

void Player::Update()
{
    if (InputManager::IsKeyHeld(KEY_A))
    {
        direction = left;
		transform->setPosition().x -= speed * Time::deltaTime();
    }
    else if (InputManager::IsKeyHeld(KEY_D))
    {
        direction = right;
		transform->setPosition().x += speed * Time::deltaTime();
    }
    else if (InputManager::IsKeyHeld(KEY_W))
    {
        direction = up;
		transform->setPosition().y += speed * Time::deltaTime();
    }
    else if (InputManager::IsKeyHeld(KEY_S))
    {
        direction = down;
		transform->setPosition().y -= speed * Time::deltaTime();
    }
    else direction = idle;
}

void Player::setUpAnimations() 
{
    SpriteAnimation* idleDown = new SpriteAnimation(*sprite, 0.6f);
    idleDown->AddFrame(SpriteKeyframe("player_idle_down_0", 0.2f));
    idleDown->AddFrame(SpriteKeyframe("player_idle_down_1", 0.2f));
    idleDown->AddFrame(SpriteKeyframe("player_idle_down_2", 0.2f));

    SpriteAnimation* idleUp = new SpriteAnimation(*sprite, 0.6f);
    idleUp->AddFrame(SpriteKeyframe("player_idle_up_0", 0.2f));
    idleUp->AddFrame(SpriteKeyframe("player_idle_up_1", 0.2f));
    idleUp->AddFrame(SpriteKeyframe("player_idle_up_2", 0.2f));

    SpriteAnimation* idleRight = new SpriteAnimation(*sprite, 0.6f);
    idleRight->AddFrame(SpriteKeyframe("player_idle_right_0", 0.2f));
    idleRight->AddFrame(SpriteKeyframe("player_idle_right_1", 0.2f));
    idleRight->AddFrame(SpriteKeyframe("player_idle_right_2", 0.2f));

    SpriteAnimation* idleLeft = new SpriteAnimation(*sprite, 0.6f);
    idleLeft->AddFrame(SpriteKeyframe("player_idle_left_0", 0.2f));
    idleLeft->AddFrame(SpriteKeyframe("player_idle_left_1", 0.2f));
    idleLeft->AddFrame(SpriteKeyframe("player_idle_left_2", 0.2f));

    SpriteAnimation* walkDown = new SpriteAnimation(*sprite);
    for (int i = 0; i <= 9; ++i)
        walkDown->AddFrame(SpriteKeyframe("player_walk_down_" + std::to_string(i), 0.1f));

    SpriteAnimation* walkUp = new SpriteAnimation(*sprite);
    for (int i = 0; i <= 9; ++i)
        walkUp->AddFrame(SpriteKeyframe("player_walk_up_" + std::to_string(i), 0.1f));

    SpriteAnimation* walkLeft = new SpriteAnimation(*sprite);
    for (int i = 0; i <= 9; ++i)
        walkLeft->AddFrame(SpriteKeyframe("player_walk_left_" + std::to_string(i), 0.1f));

    SpriteAnimation* walkRight = new SpriteAnimation(*sprite);
    for (int i = 0; i <= 9; ++i)
        walkRight->AddFrame(SpriteKeyframe("player_walk_right_" + std::to_string(i), 0.1f));

    animator->AddState("IdleDown", idleDown);
    animator->AddState("IdleUp", idleUp);
    animator->AddState("IdleRight", idleRight);
    animator->AddState("IdleLeft", idleLeft);

    animator->AddState("WalkDown", walkDown);
    animator->AddState("WalkUp", walkUp);
    animator->AddState("WalkLeft", walkLeft);
    animator->AddState("WalkRight", walkRight);

    animator->AddTransition("IdleDown", "WalkLeft", [this]() {return direction == left; });
    animator->AddTransition("IdleDown", "WalkRight", [this]() {return direction == right; });
    animator->AddTransition("IdleDown", "WalkUp", [this]() {return direction == up; });
    animator->AddTransition("IdleDown", "WalkDown", [this]() {return direction == down; });

    animator->AddTransition("WalkLeft", "IdleDown", [this]() {return direction == idle; });
    animator->AddTransition("WalkLeft", "WalkRight", [this]() {return direction == right; });
    animator->AddTransition("WalkLeft", "WalkUp", [this]() {return direction == up; });
    animator->AddTransition("WalkLeft", "WalkDown", [this]() {return direction == down; });

	animator->AddTransition("WalkRight", "IdleDown", [this]() {return direction == idle; });
	animator->AddTransition("WalkRight", "WalkLeft", [this]() {return direction == left; });
	animator->AddTransition("WalkRight", "WalkUp", [this]() {return direction == up; });
	animator->AddTransition("WalkRight", "WalkDown", [this]() {return direction == down; });

	animator->AddTransition("WalkUp", "IdleDown", [this]() {return direction == idle; });
	animator->AddTransition("WalkUp", "WalkLeft", [this]() {return direction == left; });
	animator->AddTransition("WalkUp", "WalkRight", [this]() {return direction == right; });
	animator->AddTransition("WalkUp", "WalkDown", [this]() {return direction == down; });

	animator->AddTransition("WalkDown", "IdleDown", [this]() {return direction == idle; });
	animator->AddTransition("WalkDown", "WalkLeft", [this]() {return direction == left; });
	animator->AddTransition("WalkDown", "WalkRight", [this]() {return direction == right; });
	animator->AddTransition("WalkDown", "WalkUp", [this]() {return direction == up; });
}

