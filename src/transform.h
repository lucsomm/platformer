#pragma once

#include <glm/vec2.hpp>
#include <raymath.h>

namespace platformer {
    inline Vector2 to_ray_vec(const glm::vec2& vec) {
        return Vector2{vec.x, vec.y};
    }

    inline glm::vec2 to_glm_vec(const Vector2& vec) {
        return glm::vec2{vec.x, vec.y};
    }

    struct Transform {
        Transform() = default;

        explicit Transform(const glm::vec2& position) : position{position} {
        }

        void update_previous_position() {
            previous_position = position;
        }

        [[nodiscard]] glm::vec2 interpolated_position(const float alpha) const {
            return previous_position + (position - previous_position) * alpha;
        }

        glm::vec2 position{};

    private:
        glm::vec2 previous_position{};
    };
}
