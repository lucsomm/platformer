#include "physics.h"
#include "raylib.h"
#include "entity.h"

void platformer::AABBCollider::debug_draw(const glm::vec2 position) const {
    DrawRectangleLines(
        static_cast<int>(position.x + center.x - extents.x),
        static_cast<int>(position.y + center.y - extents.y),
        static_cast<int>(extents.x * 2),
        static_cast<int>(extents.y * 2),
        RED
    );
}
