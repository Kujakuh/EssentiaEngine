#include "headers/player.hpp"

Player::Player(Essentia::Scene* scene) : entity(scene->CreateEntity("Player_main"))
{
    entity->onUpdate = [this]() { Update(); };
    EventSystem::addListener(this);

    entity->AddComponent<Sprite>(RESOURCES_PATH "GameDemo/link.png");
    auto sprite = entity->GetComponent<Sprite>();
    sprite->getTexture()->loadUVsFromJSON(RESOURCES_PATH "GameDemo/link.json");
    entity->AddComponent<Animator>();
    auto animator = entity->GetComponent<Animator>();
	auto transform = entity->GetComponent<Transform>();
	transform->setScale(Vector3(20.0f));

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

    animator->AddTransition("IdleDown", "WalkRight", []() {
        return InputManager::IsKeyPressed(KEY_P);
        });
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
    // Lógica de actualización del jugador
}
