#include "game.h"

#include <algorithm>
#include <chrono>

#include "raylib.h"
#include "player.h"

platformer::Game::StateGameplay::StateGameplay() {
    tile_map.debug_create_center_platform();
    auto& player = Player::create(tile_map);
    player.position = glm::vec2{tile_map.get_size().x / 2 * TileMap::TILE_SIZE, 32};

    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    get_singleton().main_camera.target = Vector2{0, 0};
    get_singleton().main_camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    get_singleton().main_camera.rotation = 0.0f;
    get_singleton().main_camera.zoom = 1.0f;
}

void platformer::Game::StateGameplay::update(const float delta) {
    EntityBase::update_all(delta);
}

void platformer::Game::StateGameplay::physics_update(const float delta) {
    EntityBase::physics_update_all(delta);
}

void platformer::Game::StateGameplay::draw(const float alpha) {
    ClearBackground(Color{50, 50, 50, 255});

    BeginMode2D(get_singleton().main_camera);

    EntityBase::draw_all(alpha);
    tile_map.debug_draw();

    EndMode2D();
}

platformer::Game::Game() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Platformer");

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
    state_machine.get_current_state().update(delta);
}

void platformer::Game::physics_update(const float delta) {
    state_machine.get_current_state().physics_update(delta);
}

void platformer::Game::draw(const float alpha) {
    BeginDrawing();

    state_machine.get_current_state().draw(alpha);

    EndDrawing();
}

void platformer::Game::run() {
    state_machine.change_state<StateGameplay>();

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

