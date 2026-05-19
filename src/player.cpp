#include "raylib.h"
#include "player.h"

#include "game.h"

void platformer::Player::Walking::update(float delta) {
    player.poll_input_dir();

    if (IsKeyPressed(KEY_SPACE) && player.body.is_on_floor()) {
        player.locomotion_machine.change_state<Airborne>(std::ref(player), true);
    }

    if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        player.action_machine.change_state<ActionStabbing>(std::ref(player));
    }
}

void platformer::Player::Walking::physics_update(const float delta) {
    player.default_movement(delta);

    if (!player.body.is_on_floor()) {
        player.locomotion_machine.change_state<Airborne>(std::ref(player), false);
    }
}

void platformer::Player::Airborne::update(float delta) {
    player.poll_input_dir();

    if (jumped && !IsKeyDown(KEY_SPACE)) {
        player.body.velocity.y *= JUMP_RELEASE;
        jumped = false;
    }

    if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        player.action_machine.change_state<ActionStabbing>(std::ref(player));
    }
}

void platformer::Player::Airborne::physics_update(const float delta) {
    player.default_movement(delta);

    if (player.body.is_on_floor()) {
        player.locomotion_machine.change_state<Walking>(std::ref(player));
    }
}

void platformer::Player::ActionNeutral::update(float delta) {
    State::update(delta);
}

void platformer::Player::ActionNeutral::physics_update(float delta) {
    player.update_h_facing();
    player.update_spear_dir();
}

void platformer::Player::ActionNeutral::draw(glm::vec2 draw_position) {
    State::draw(draw_position);
}

void platformer::Player::ActionStabbing::update(const float delta) {
    player.poll_input_dir();

    if (!IsKeyDown(KEY_RIGHT_SHIFT)) {
        player.action_machine.change_state<ActionNeutral>(std::ref(player));
    }
}

void platformer::Player::ActionStabbing::physics_update(float delta) {
}

void platformer::Player::ActionStabbing::draw(const glm::vec2 draw_position) {
    DrawLineV(to_ray_vec(draw_position), to_ray_vec(draw_position + player.spear_dir * SPEAR_LENGTH), GREEN);
}

void platformer::Player::update_impl(const float delta) {
    locomotion_machine.get_current_state().update(delta);
    action_machine.get_current_state().update(delta);
}

void platformer::Player::physics_update_impl(const float delta) {
    locomotion_machine.get_current_state().physics_update(delta);
    action_machine.get_current_state().physics_update(delta);
}

void platformer::Player::draw_impl(const glm::vec2 draw_position) {
    body.collider.debug_draw(draw_position);
    locomotion_machine.get_current_state().draw(draw_position);
    action_machine.get_current_state().draw(draw_position);
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
    body.velocity.x = input_dir.x * walkSpeed;
    body.velocity.y += GRAVITY * gravity_scale * delta;
    body.move_and_slide(delta, position, tile_map);
}
