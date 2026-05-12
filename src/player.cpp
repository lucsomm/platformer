#include "player.h"

void platformer::Player::physics_update(const float delta) {
    state_machine.get_current_state().physics_update(delta);
    transform.position += velocity * delta;
}
