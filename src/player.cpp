#include "raylib.h"
#include "player.h"

#include "game.h"

void platformer::Player::StateWalking::update(float delta) {
    player.poll_input_dir();

    if (IsKeyPressed(KEY_SPACE) && player.body.is_on_floor()) {
        player.state_machine.change_state<StateAirborne>(std::ref(player), true);
    }

    if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        player.state_machine.change_state<StateStabSpear>(std::ref(player));
    }
}

void platformer::Player::StateWalking::physics_update(const float delta) {
    player.default_movement(delta);

    if (!player.body.is_on_floor()) {
        player.state_machine.change_state<StateAirborne>(std::ref(player), false);
    }
}

void platformer::Player::StateAirborne::update(float delta) {
    player.poll_input_dir();

    if (jumped && !IsKeyDown(KEY_SPACE)) {
        player.body.velocity.y *= JUMP_RELEASE;
        jumped = false;
    }

    if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        player.state_machine.change_state<StateStabSpear>(std::ref(player));
    }
}

void platformer::Player::StateAirborne::physics_update(const float delta) {
    player.default_movement(delta);

    if (player.body.is_on_floor()) {
        player.state_machine.change_state<StateWalking>(std::ref(player));
    }
}

void platformer::Player::StateStabSpear::update(const float delta) {
    player.poll_input_dir();
}

void platformer::Player::StateStabSpear::physics_update(const float delta) {
    player.body.velocity.x = player.input_dir.x * player.walkSpeed;
    player.body.velocity.y += GRAVITY * player.gravity_scale * delta;
    player.body.move_and_slide(delta, player.position, player.tile_map);
}

void platformer::Player::StateStabSpear::draw(const glm::vec2 draw_position) {
    DrawLineV(to_ray_vec(draw_position), to_ray_vec(draw_position + player.spear_dir * SPEAR_LENGTH), GREEN);
}

void platformer::Player::update_impl(const float delta) {
    state_machine.get_current_state().update(delta);
}

void platformer::Player::physics_update_impl(const float delta) {
    state_machine.get_current_state().physics_update(delta);
}

void platformer::Player::draw_impl(const glm::vec2 draw_position) {
    body.collider.debug_draw(draw_position);
    state_machine.get_current_state().draw(draw_position);
    debug_draw_spear_marker(draw_position);
}

void platformer::Player::poll_input_dir() {
    input_dir = glm::vec2{
        static_cast<float>(IsKeyDown(KEY_D)) - static_cast<float>(IsKeyDown(KEY_A)),
        static_cast<float>(IsKeyDown(KEY_S)) - static_cast<float>(IsKeyDown(KEY_W))
    };
}

void platformer::Player::update_spear_dir() {
    if (input_dir.y != 0.f) {
        spear_dir = glm::vec2{0.f, 1.f} * input_dir.y;
    } else {
        spear_dir = glm::vec2{1.f, 0.f} * h_facing;
    }
}

void platformer::Player::debug_draw_spear_marker(const glm::vec2 draw_position) const {
    if (spear_dir.y != 0.f) {
        DrawCircleV(to_ray_vec(draw_position + spear_dir * body.collider.extents.y), 4, YELLOW);
    } else {
        DrawCircleV(to_ray_vec(draw_position + spear_dir * body.collider.extents.x), 4, YELLOW);
    }
}

void platformer::Player::default_movement(const float delta) {
    if (input_dir.x != 0.f) {
        h_facing = input_dir.x;
    }

    update_spear_dir();

    body.velocity.x = input_dir.x * walkSpeed;
    body.velocity.y += GRAVITY * gravity_scale * delta;
    body.move_and_slide(delta, position, tile_map);
}
