#pragma once
#include "raylib.h"
#include "state_machine.h"
#include "tile_map.h"

namespace platformer {
    class Game {
    public:
        struct StateGameplay final : State {
            StateGameplay();

            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(float alpha) override;

            TileMap tile_map{16, 16};
        };

        static Game& get_singleton() {
            static Game game;
            return game;
        }

        Game();

        Game(const Game&) = delete;

        Game& operator=(const Game&) = delete;

        Game& operator=(Game&&) = delete;

        Game(Game&&) = delete;

        ~Game();

        void run();

        // Start with empty state as to not init camera before window
        StateMachine<State, StateGameplay> state_machine;
        Camera2D main_camera;

    private:
        void update(float delta);

        void physics_update(float delta);

        void draw(float alpha);

        [[nodiscard]] double get_physics_dt() const {
            return 1. / physics_rate;
        }

        double physics_rate = 60.;
    };
}

