#pragma once
#include "physics.h"
#include "entity.h"
#include "state_machine.h"

namespace platformer {
    class TileMap;

    class Player : public Entity<Player> {
    public:
        class StateWalking final : public State {
        public:
            explicit StateWalking(Player& player) : player{player} {
            }

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(glm::vec2 draw_position) override;

        private:
            Player& player;
        };

        class StateAirborne final : public State {
        public:
            explicit StateAirborne(Player& player, const bool jumped) : player{player}, jumped{jumped} {
                if (jumped) {
                    player.body.velocity.y = -JUMP_HEIGHT;
                }
            }

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(glm::vec2 draw_position) override;

        private:
            Player& player;
            bool jumped{};
        };

        explicit Player(const TileMap& tile_map) : tile_map(tile_map) {
        }

        void update_impl(float delta);

        void physics_update_impl(float delta);

        void draw_impl(glm::vec2 draw_position);

    private:
        static constexpr float GRAVITY = 800.f;
        static constexpr float JUMP_HEIGHT = 400.f;
        static constexpr float JUMP_RELEASE = .6f;

        void poll_input_dir();

        void debug_draw_spear_marker(glm::vec2 draw_position) const;

        StateMachine<StateWalking, StateAirborne> state_machine{*this};
        PhysicsBody body{AABBCollider{{16.f, 16.f}}};
        const TileMap& tile_map;
        glm::vec2 input_dir{};
        float h_facing{1};
        float walkSpeed = 200.f;
        float gravity_scale = 1.f;
    };
}
