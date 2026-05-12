#pragma once
#include "state_machine.h"
#include "transform.h"

namespace platformer {
    struct Player {
        class StateWalk final : public State {
        };

        void physics_update(float delta);

        StateMachine<StateWalk> state_machine;
        Transform transform;
        glm::vec2 velocity{};
    };
}
