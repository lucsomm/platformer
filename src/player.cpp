#include "raylib.h"
#include "player.h"

#include "game.h"

void platformer::Player::StateMove::update(float delta) {
    if (IsKeyPressed(KEY_SPACE) && player.collider.is_colliding(player.position + glm::vec2{0, 2}, player.tile_map)) {
        player.velocity.y = -JUMP_HEIGHT;
    }
}

void platformer::Player::StateMove::physics_update(const float delta) {
    player.velocity.x = player.input_dir.x * player.walkSpeed;
    player.velocity.y += GRAVITY * player.gravity_scale * delta;
}

void platformer::Player::StateMove::draw(const glm::vec2 draw_position) {
    player.collider.debug_draw(draw_position);
}

void platformer::Player::update_impl(const float delta) {
    input_dir = glm::vec2{
        static_cast<float>(IsKeyDown(KEY_D)) - static_cast<float>(IsKeyDown(KEY_A)),
        static_cast<float>(IsKeyDown(KEY_S)) - static_cast<float>(IsKeyDown(KEY_W))
    };

    state_machine.get_current_state().update(delta);
}

void platformer::Player::physics_update_impl(const float delta) {
    state_machine.get_current_state().physics_update(delta);
    move_and_slide(delta, position, velocity, collider, tile_map);
}

void platformer::Player::draw_impl(const glm::vec2 draw_position) {
    state_machine.get_current_state().draw(draw_position);
}
