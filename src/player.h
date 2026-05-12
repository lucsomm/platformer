#pragma once
#include "state_machine.h"
#include "transform.h"

namespace platformer {
    struct Player {
        void physics_update(float delta);

        Transform transform;
    };
}
