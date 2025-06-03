#include <EssentiaEngine>

class Player : EventListener
{
    public:
        GameObject entity;

        Player(Essentia::Scene* scene) : entity(scene->CreateEntity("Player_main"))
        {
            entity->onUpdate = [this]() {Update(); };
            EventSystem::addListener(this);

            entity->AddComponent<Sprite>(RESOURCES_PATH "GameDemo/link.png");
			auto sprite = entity->GetComponent<Sprite>();
            sprite->getTexture()->loadUVsFromJSON(RESOURCES_PATH "GameDemo/link.json");
			entity->AddComponent<Animator>();

			auto animator = entity->GetComponent<Animator>();

            SpriteAnimation* attackAnimation = new SpriteAnimation(*sprite);
			SpriteAnimation* idleAnimation = new SpriteAnimation(*sprite, 0.9f);

            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_0", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_1", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_2", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_3", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_4", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_5", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_6", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_7", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_8", 0.1f));
            attackAnimation->AddFrame(SpriteKeyframe("player_walk_right_9", 0.1f));

            idleAnimation->AddFrame(SpriteKeyframe("player_idle_down_0", 0.3f));
            idleAnimation->AddFrame(SpriteKeyframe("player_idle_down_1", 0.3f));
            idleAnimation->AddFrame(SpriteKeyframe("player_idle_down_2", 0.3f));

			animator->AddState("Idle", idleAnimation);
            animator->AddState("AttackDown", attackAnimation);
			animator->AddTransition("Idle", "AttackDown", []() {
                return InputManager::IsKeyHeld(KEY_P);
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
