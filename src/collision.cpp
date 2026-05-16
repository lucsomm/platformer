#include "collision.h"
#include "raylib.h"
#include "entity.h"

void platformer::AABBCollider::debug_draw(const glm::vec2 position) const {
    DrawRectangleLines(
        static_cast<int>(position.x + center.x),
        static_cast<int>(position.y + center.y),
        static_cast<int>(extents.x),
        static_cast<int>(extents.y),
        RED
    );
}
