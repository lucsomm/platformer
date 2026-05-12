#pragma once


namespace platformer {
    class Game {
    public:
        Game();

        Game(const Game&) = delete;

        Game& operator=(const Game&) = delete;

        Game& operator=(Game&&) = delete;

        Game(Game&&) = delete;

        ~Game();

        void run();

    private:
        void update(float delta);

        void physics_update(float delta);

        void draw(float alpha);

        [[nodiscard]] double get_physics_dt() const {
            return 1. / physics_rate;
        }

        double physics_rate = 120.;
    };
}

