#include "raylib.h"
#include "player.h"

void platformer::Player::StateWalk::physics_update(const float delta) {
    const glm::vec2 dir{
        static_cast<float>(IsKeyDown(KEY_D)) - static_cast<float>(IsKeyDown(KEY_A)),
        static_cast<float>(IsKeyDown(KEY_S)) - static_cast<float>(IsKeyDown(KEY_W))
    };

    player.velocity.x = dir.x * player.walkSpeed;
    player.position += player.velocity * delta;
}

void platformer::Player::StateWalk::draw(glm::vec2 draw_position) {
}

void platformer::Player::physics_update_impl(float delta) {
    state_machine.get_current_state().physics_update(delta);
}

void platformer::Player::draw_impl(glm::vec2 draw_position) {
}
