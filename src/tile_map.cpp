#include "tile_map.h"

#include "raylib.h"

void platformer::TileMap::debug_draw() const {
    for (size_t i = 0; i < tiles.size(); i++) {
        if (tiles[i] == 0) {
            continue;
        }

        const auto pos = to_position(i);
        DrawRectangleLines(pos.x, pos.y, pos.x + TILE_SIZE, pos.y + TILE_SIZE, RAYWHITE);
    }
}

void platformer::TileMap::debug_create_center_platform() {
    const auto center = static_cast<int>(tiles.size() / 2);
    constexpr int width = 5;

    for (int i = center - width; i < center + width; i++) {
        tiles[i] = 1;
    }
}
