#include "player.h"

void platformer::Player::StateWalk::physics_update(const float delta) {
}

void platformer::Player::StateWalk::draw(const float alpha) {
}

void platformer::Player::physics_update(const float delta) {
    state_machine.get_current_state().physics_update(delta);
    transform.position += velocity * delta;
}
