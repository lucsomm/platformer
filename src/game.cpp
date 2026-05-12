#include "game.h"

#include <algorithm>
#include <chrono>

#include "raylib.h"

platformer::Game::Game() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    const auto monitor = GetCurrentMonitor();

    if (monitor < 0) {
        SetTargetFPS(60);
        return;
    }

    SetTargetFPS(GetMonitorRefreshRate(monitor));
}

platformer::Game::~Game() {
    CloseWindow();
}

void platformer::Game::update(const float delta) {
}

void platformer::Game::physics_update(float delta) {
}

void platformer::Game::draw(const float alpha) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void platformer::Game::run() {
    double physics_accumulator = 0.;

    using clock = std::chrono::high_resolution_clock;
    auto previous_time = clock::now();

    while (!WindowShouldClose()) {
        constexpr double SPIRAL_OF_DEATH_THRESHOLD = .2;

        const auto current_time = clock::now();
        const std::chrono::duration<double> time_span = current_time - previous_time;
        const double dt = time_span.count();
        previous_time = current_time;

        const double physics_dt = get_physics_dt();
        physics_accumulator = std::min(physics_accumulator + dt, SPIRAL_OF_DEATH_THRESHOLD);

        update(static_cast<float>(dt));

        while (physics_accumulator >= physics_dt) {
            physics_update(static_cast<float>(physics_dt));
            physics_accumulator -= physics_dt;
        }

        const auto alpha = physics_accumulator / physics_dt;

        draw(static_cast<float>(alpha));
    }
}

