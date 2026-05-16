#pragma once
#include "entity.h"
#include "state_machine.h"

namespace platformer {
    class Player : public Entity<Player> {
    public:
        class StateWalk final : public State {
        public:
            explicit StateWalk(Player& player) : player{player} {
            }

            void physics_update(float delta) override;

            void draw(glm::vec2 draw_position) override;

        private:
            Player& player;
        };

        void update_impl(float delta) {
        }

        void physics_update_impl(float delta);

        void draw_impl(glm::vec2 draw_position);

    private:
        StateMachine<StateWalk> state_machine{*this};
        Transform transform;
        glm::vec2 velocity{};
        float walkSpeed = 200.f;
    };
}
