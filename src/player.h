#pragma once
#include "physics.h"
#include "entity.h"
#include "state_machine.h"

namespace platformer {
    class TileMap;

    class Player : public Entity<Player> {
    public:
        class Walking final : public State {
        public:
            explicit Walking(Player& player) : player{player} {
            }

            void update(float delta) override;

            void physics_update(float delta) override;

        private:
            Player& player;
        };

        class Airborne final : public State {
        public:
            explicit Airborne(Player& player, const bool jumped) : player{player}, jumped{jumped} {
                if (jumped) {
                    player.body.velocity.y = -JUMP_HEIGHT;
                }
            }

            void update(float delta) override;

            void physics_update(float delta) override;

        private:
            Player& player;
            bool jumped{};
        };

        class ActionNeutral final : public State {
        public:
            explicit ActionNeutral(Player& player) : player{player} {
            }

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(glm::vec2 draw_position) override;

        private:
            Player& player;
        };

        class ActionStabbing final : public State {
        public:
            explicit ActionStabbing(Player& player) : player{player} {
            }

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(glm::vec2 draw_position) override;

        private:
            Player& player;
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
        static constexpr float SPEAR_LENGTH = TileMap::TILE_SIZE * 2;

        void poll_input_dir();

        void update_spear_dir();

        void debug_draw_spear_marker(glm::vec2 draw_position) const;

        void update_h_facing() {
            if (input_dir.x != 0.f) {
                h_facing = input_dir.x;
            }
        }

        void default_movement(float delta);

        StateMachine<Walking, Airborne> locomotion_machine{*this};
        StateMachine<ActionNeutral, ActionStabbing> action_machine{*this};
        PhysicsBody body{AABBCollider{{16.f, 16.f}}};
        const TileMap& tile_map;
        glm::vec2 input_dir{};
        glm::vec2 spear_dir{1.f, 0.f};
        float h_facing{1};
        float walkSpeed = 200.f;
        float gravity_scale = 1.f;
    };
}
