#pragma once


namespace platformer {
    class Game {
    public:
        Game();

        ~Game();

        void run();

    private:
        void update(float delta);

        void draw(float alpha);
    };
}

