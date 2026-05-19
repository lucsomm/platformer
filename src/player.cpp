#include "raylib.h"
#include "player.h"

#include "game.h"

void platformer::Player::StateWalking::update(float delta) {
    player.poll_input_dir();

    if (IsKeyPressed(KEY_SPACE) && player.body.is_on_floor()) {
        player.state_machine.change_state<StateAirborne>(std::ref(player), true);
    }
}

void platformer::Player::StateWalking::physics_update(const float delta) {
    if (player.input_dir.x != 0.f) {
        player.h_facing = player.input_dir.x;
    }

    player.body.velocity.x = player.input_dir.x * player.walkSpeed;
    player.body.velocity.y += GRAVITY * player.gravity_scale * delta;
    player.body.move_and_slide(delta, player.position, player.tile_map);
}

void platformer::Player::StateWalking::draw(const glm::vec2 draw_position) {
    player.body.collider.debug_draw(draw_position);
}

void platformer::Player::StateAirborne::update(float delta) {
    player.poll_input_dir();

    if (jumped && IsKeyReleased(KEY_SPACE)) {
        player.body.velocity.y *= JUMP_RELEASE;
        jumped = false;
    }
}

void platformer::Player::StateAirborne::physics_update(const float delta) {
    if (player.input_dir.x != 0.f) {
        player.h_facing = player.input_dir.x;
    }

    player.body.velocity.x = player.input_dir.x * player.walkSpeed;
    player.body.velocity.y += GRAVITY * player.gravity_scale * delta;
    player.body.move_and_slide(delta, player.position, player.tile_map);

    if (player.body.is_on_floor()) {
        player.state_machine.change_state<StateWalking>(std::ref(player));
    }
}

void platformer::Player::StateAirborne::draw(const glm::vec2 draw_position) {
    player.body.collider.debug_draw(draw_position);
}

void platformer::Player::update_impl(const float delta) {
    state_machine.get_current_state().update(delta);
}

void platformer::Player::physics_update_impl(const float delta) {
    state_machine.get_current_state().physics_update(delta);
}

void platformer::Player::draw_impl(const glm::vec2 draw_position) {
    state_machine.get_current_state().draw(draw_position);

    debug_draw_spear_marker(draw_position);
}

void platformer::Player::poll_input_dir() {
    input_dir = glm::vec2{
        static_cast<float>(IsKeyDown(KEY_D)) - static_cast<float>(IsKeyDown(KEY_A)),
        static_cast<float>(IsKeyDown(KEY_S)) - static_cast<float>(IsKeyDown(KEY_W))
    };
}

void platformer::Player::debug_draw_spear_marker(const glm::vec2 draw_position) const {
    if (input_dir.y != 0.f) {
        DrawCircleV(to_ray_vec(draw_position + glm::vec2{0.f, input_dir.y * body.collider.extents.y}), 4, YELLOW);
    } else {
        DrawCircleV(to_ray_vec(draw_position + glm::vec2{h_facing * body.collider.extents.x, 0.f}), 4, YELLOW);
    }
}
