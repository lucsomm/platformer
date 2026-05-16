#pragma once
#include "state_machine.h"

namespace platformer {
    class Game {
    public:
        class StateGameplay final : public State {
            void update(float delta) override;

            void physics_update(float delta) override;

            void draw(float alpha) override;
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

        StateMachine<StateGameplay> state_machine;
        // Have a gameplay state that hosts levels as states too?

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

