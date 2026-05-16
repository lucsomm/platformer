#pragma once
#include <cstdint>
#include <vector>

#include "collision.h"
#include "glm/vec2.hpp"

namespace platformer {
    class TileMap {
    public:
        using Tile = uint8_t;
        static constexpr int TILE_SIZE = 16;

        [[nodiscard]] int to_index(const glm::ivec2 position) const {
            return position.y * static_cast<int>(map_size.x) + position.x;
        }

        [[nodiscard]] const Tile& get_tile(const glm::ivec2 position) const {
            return tiles[to_index(position)];
        }

        [[nodiscard]] Tile& get_tile(const glm::ivec2 position) {
            return tiles[to_index(position)];
        }

        [[nodiscard]] bool is_colliding(const glm::vec2 position, const AABBCollider& collider) const {
            const glm::vec2 min_bounds = position + collider.center - collider.extents;
            const glm::vec2 max_bounds = position + collider.center + collider.extents;

            const auto start_tile = glm::ivec2{
                static_cast<int>(min_bounds.x) / TILE_SIZE,
                static_cast<int>(min_bounds.y) / TILE_SIZE
            };

            const auto end_tile = glm::ivec2{
                static_cast<int>(max_bounds.x) / TILE_SIZE,
                static_cast<int>(max_bounds.y) / TILE_SIZE
            };

            for (int y = start_tile.y; y <= end_tile.y; ++y) {
                for (int x = start_tile.x; x <= end_tile.x; ++x) {
                    if (get_tile({x, y}) > 0) {
                        return true;
                    }
                }
            }

            return false;
        }

    private:
        glm::vec2 map_size{};
        std::vector<Tile> tiles;
    };
}
