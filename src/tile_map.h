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

        [[nodiscard]] glm::ivec2 to_position(const int index) const {
            return glm::ivec2{index % map_size.x, index / map_size.x} * TILE_SIZE;
        }

        [[nodiscard]] const Tile& get_tile(const glm::ivec2 position) const {
            return tiles[to_index(position)];
        }

        [[nodiscard]] Tile& get_tile(const glm::ivec2 position) {
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
