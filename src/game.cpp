#include "game.h"
#include "raylib.h"

platformer::Game::Game() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
}

platformer::Game::~Game() {
    CloseWindow();
}

void platformer::Game::update(const float delta) {
}

void platformer::Game::draw(const float alpha) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void platformer::Game::run() {
    while (!WindowShouldClose()) {
        update(1.f);
        draw(1.f);
    }
}

