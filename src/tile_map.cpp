#include "tile_map.h"

#include "raylib.h"

void platformer::TileMap::debug_draw() const {
    for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i] == 0) {
            continue;
        }

        const auto pos = to_position(i);
        DrawRectangleLines(pos.x, pos.y, TILE_SIZE, TILE_SIZE, RAYWHITE);
    }

    DrawRectangleLines(0, 0, map_size.x * TILE_SIZE, map_size.y * TILE_SIZE, GRAY);
}

void platformer::TileMap::debug_create_center_platform() {
    const auto center = to_index(glm::vec2{map_size.x / 2, map_size.y / 2});
    const auto width = std::min(3, map_size.x);

    for (int i = center - width; i < center + width; i++) {
        tiles[i] = 1;
    }

    tiles[center - map_size.x] = 1;
}
