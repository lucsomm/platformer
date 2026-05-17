#pragma once
#include "glm/vec2.hpp"

namespace platformer {
    struct AABBCollider {
        explicit AABBCollider(const glm::vec2 extents, const glm::vec2 center = {}) : extents{extents}, center{center} {
        }

        void debug_draw(glm::vec2 position) const;

        glm::vec2 extents{};
        glm::vec2 center{};
    };
}
