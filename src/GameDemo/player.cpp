#include <EssentiaEngine>

class Player : EventListener
{
public:
    GameObject entity;

    Player(Essentia::Scene* scene) : entity(scene->CreateEntity("Player_main"))
    {
        entity->onUpdate = [this]() { Update(); };
        EventSystem::addListener(this);

        entity->AddComponent<Sprite>(RESOURCES_PATH "GameDemo/link.png");
        auto sprite = entity->GetComponent<Sprite>();
        sprite->getTexture()->loadUVsFromJSON(RESOURCES_PATH "GameDemo/link.json");
        entity->AddComponent<Animator>();

        auto animator = entity->GetComponent<Animator>();

        // Idle down
        SpriteAnimation* idleDown = new SpriteAnimation(*sprite, 0.6f);
        idleDown->AddFrame(SpriteKeyframe("player_idle_down_0", 0.2f));
        idleDown->AddFrame(SpriteKeyframe("player_idle_down_1", 0.2f));
        idleDown->AddFrame(SpriteKeyframe("player_idle_down_2", 0.2f));

        // Idle up
        SpriteAnimation* idleUp = new SpriteAnimation(*sprite, 0.6f);
        idleUp->AddFrame(SpriteKeyframe("player_idle_up_0", 0.2f));
        idleUp->AddFrame(SpriteKeyframe("player_idle_up_1", 0.2f));
        idleUp->AddFrame(SpriteKeyframe("player_idle_up_2", 0.2f));

        // Idle right
        SpriteAnimation* idleRight = new SpriteAnimation(*sprite, 0.6f);
        idleRight->AddFrame(SpriteKeyframe("player_idle_right_0", 0.2f));
        idleRight->AddFrame(SpriteKeyframe("player_idle_right_1", 0.2f));
        idleRight->AddFrame(SpriteKeyframe("player_idle_right_2", 0.2f));

        // Idle left
        SpriteAnimation* idleLeft = new SpriteAnimation(*sprite, 0.6f);
        idleLeft->AddFrame(SpriteKeyframe("player_idle_left_0", 0.2f));
        idleLeft->AddFrame(SpriteKeyframe("player_idle_left_1", 0.2f));
        idleLeft->AddFrame(SpriteKeyframe("player_idle_left_2", 0.2f));

        // Walk down
        SpriteAnimation* walkDown = new SpriteAnimation(*sprite);
        for (int i = 0; i <= 9; ++i)
            walkDown->AddFrame(SpriteKeyframe("player_walk_down_" + std::to_string(i), 0.1f));

        // Walk up
        SpriteAnimation* walkUp = new SpriteAnimation(*sprite);
        for (int i = 0; i <= 9; ++i)
            walkUp->AddFrame(SpriteKeyframe("player_walk_up_" + std::to_string(i), 0.1f));

        // Walk left
        SpriteAnimation* walkLeft = new SpriteAnimation(*sprite);
        for (int i = 0; i <= 9; ++i)
            walkLeft->AddFrame(SpriteKeyframe("player_walk_left_" + std::to_string(i), 0.1f));

        // Walk right
        SpriteAnimation* walkRight = new SpriteAnimation(*sprite);
        for (int i = 0; i <= 9; ++i)
            walkRight->AddFrame(SpriteKeyframe("player_walk_right_" + std::to_string(i), 0.1f));

        // Add states
        animator->AddState("IdleDown", idleDown);
        animator->AddState("IdleUp", idleUp);
        animator->AddState("IdleRight", idleRight);
        animator->AddState("IdleLeft", idleLeft);

        animator->AddState("WalkDown", walkDown);
        animator->AddState("WalkUp", walkUp);
        animator->AddState("WalkLeft", walkLeft);
        animator->AddState("WalkRight", walkRight);  

        // Example transition
        animator->AddTransition("IdleDown", "WalkRight", []() {
            return InputManager::IsKeyPressed(KEY_P);
            });
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
