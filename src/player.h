#pragma once
#include "entity.h"
#include "state_machine.h"
#include "transform.h"

namespace platformer {
    class Player : Entity<Player> {
        class StateWalk final : public State {
        public:
            explicit StateWalk(Player& player) : player{player} {
            }

        private:
            Player& player;
        };

        StateMachine<StateWalk> state_machine{*this};
        Transform transform;
        glm::vec2 velocity{};
    };
}
