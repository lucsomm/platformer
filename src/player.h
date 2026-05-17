#pragma once
#include "collision.h"
#include "entity.h"
#include "state_machine.h"

namespace platformer {
    class TileMap;

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

        explicit Player(const TileMap& tile_map) : tile_map(tile_map) {
        }

        void update_impl(float delta) {
        }

        void physics_update_impl(float delta);

        void draw_impl(glm::vec2 draw_position);

    private:
        static constexpr float GRAVITY = 200.f;

        StateMachine<StateWalk> state_machine{*this};
        glm::vec2 velocity{};
        AABBCollider collider{glm::vec2{32, 32}};
        const TileMap& tile_map;
        float walkSpeed = 200.f;
        float gravity_scale = 1.f;
    };
}
