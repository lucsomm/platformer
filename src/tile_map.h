#pragma once
#include <cstdint>
#include <vector>

#include "glm/vec2.hpp"

namespace platformer {
    class TileMap {
    public:
        using Tile = uint8_t;
        static constexpr int TILE_SIZE = 32;

        TileMap(const size_t size_x, const size_t size_y) : map_size{
                                                                static_cast<int>(size_x), static_cast<int>(size_y)
                                                            }, tiles(size_x * size_y) {
        }

        [[nodiscard]] int to_index(const glm::ivec2 position) const {
            return position.y * map_size.x + position.x;
        }

        [[nodiscard]] glm::vec2 to_world_position(const int index) const {
            return glm::vec2{index % map_size.x, index / map_size.x} * static_cast<float>(TILE_SIZE);
        }

        [[nodiscard]] glm::ivec2 to_tile_position(const int index) const {
            return glm::ivec2{index % map_size.x, index / map_size.x};
        }

        [[nodiscard]] const Tile& get_tile_at_world_position(glm::vec2 position) const {
            position.x /= TILE_SIZE;
            position.y /= TILE_SIZE;
            return tiles[to_index(position)];
        }

        [[nodiscard]] Tile& get_tile_at_world_position(glm::vec2 position) {
            position.x /= TILE_SIZE;
            position.y /= TILE_SIZE;
            return tiles[to_index(position)];
        }

        [[nodiscard]] const Tile& get_tile_at_tile_position(const glm::ivec2 position) const {
            return tiles[to_index(position)];
        }

        [[nodiscard]] Tile& get_tile_at_tile_position(const glm::ivec2 position) {
            return tiles[to_index(position)];
        }

        [[nodiscard]] glm::ivec2 get_size() const {
            return map_size;
        }

        void debug_draw() const;

        void debug_create_center_platform();

    private:
        glm::ivec2 map_size{};
        std::vector<Tile> tiles;
    };
}
