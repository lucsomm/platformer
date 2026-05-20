#pragma once
#include "tile_map.h"
#include "glm/vec2.hpp"

namespace platformer {
    template<typename T>
    T sign(T val) {
        return (T{} < val) - (val < T{});
    }

    struct RaycastResult {
        RaycastResult(const glm::vec2 point, const bool hit) : point{point}, hit{hit} {
        }

        glm::vec2 point{};
        bool hit{};
    };

    inline RaycastResult AARaycast(const glm::vec2 position, const glm::vec2 ray, const TileMap& tile_map) {
        const glm::vec2 dir{sign(ray.x), sign(ray.y)};
        const float ray_len = ray.x + ray.y;
        float res_len{};

        while (res_len < ray_len) {
            if (tile_map.get_tile(position + dir * res_len) > 0) {
                return RaycastResult{dir * res_len, true};
            }

            res_len += std::min(static_cast<float>(TileMap::TILE_SIZE), ray_len - res_len);
        }

        return RaycastResult{ray, false};
    }

    struct AABBCollider {
        explicit AABBCollider(const glm::vec2 extents, const glm::vec2 center = {}) : extents{extents}, center{center} {
        }

        [[nodiscard]] bool is_colliding(const glm::vec2 position, const TileMap& tile_map) const {
            const glm::vec2 min_bounds = position + center - extents;
            const glm::vec2 max_bounds = position + center + extents;

            const auto start_tile = glm::ivec2{
                static_cast<int>(min_bounds.x) / TileMap::TILE_SIZE,
                static_cast<int>(min_bounds.y) / TileMap::TILE_SIZE
            };

            const auto end_tile = glm::ivec2{
                static_cast<int>(max_bounds.x) / TileMap::TILE_SIZE,
                static_cast<int>(max_bounds.y) / TileMap::TILE_SIZE
            };

            for (int y = start_tile.y; y <= end_tile.y; ++y) {
                for (int x = start_tile.x; x <= end_tile.x; ++x) {
                    if (tile_map.get_tile({x, y}) > 0) {
                        return true;
                    }
                }
            }

            return false;
        }

        void debug_draw(glm::vec2 position) const;

        glm::vec2 extents{};
        glm::vec2 center{};
    };

    struct PhysicsBody {
        explicit PhysicsBody(const AABBCollider& collider, const glm::vec2 velocity = {}) : collider{collider},
            velocity{velocity} {
        }

        [[nodiscard]] bool is_on_floor() const {
            return on_floor;
        }

        [[nodiscard]] bool is_on_ceiling() const {
            return on_ceiling;
        }

        [[nodiscard]] bool is_on_wall() const {
            return on_wall;
        }

        void move_and_slide(const float delta, glm::vec2& position, const TileMap& tile_map) {
            constexpr float EPSILON = 0.001f;

            on_floor = false;
            on_ceiling = false;
            on_wall = false;

            position.x += velocity.x * delta;

            glm::vec2 min_bounds = position + collider.center - collider.extents;
            glm::vec2 max_bounds = position + collider.center + collider.extents;

            glm::ivec2 start_tile = {
                static_cast<int>(min_bounds.x) / TileMap::TILE_SIZE,
                static_cast<int>(min_bounds.y + EPSILON) / TileMap::TILE_SIZE
            };
            glm::ivec2 end_tile = {
                static_cast<int>(max_bounds.x) / TileMap::TILE_SIZE,
                static_cast<int>(max_bounds.y - EPSILON) / TileMap::TILE_SIZE
            };

            for (int y = start_tile.y; y <= end_tile.y; ++y) {
                for (int x = start_tile.x; x <= end_tile.x; ++x) {
                    if (tile_map.get_tile({x, y}) > 0) {
                        if (velocity.x > 0) {
                            position.x = static_cast<float>(x) * TileMap::TILE_SIZE - collider.extents.x - collider.
                                         center.
                                         x;
                        } else if (velocity.x < 0) {
                            position.x = static_cast<float>(x + 1) * TileMap::TILE_SIZE + collider.extents.x - collider.
                                         center.x;
                        }
                        velocity.x = 0;
                        on_wall = true;

                        min_bounds.x = position.x + collider.center.x - collider.extents.x;
                        max_bounds.x = position.x + collider.center.x + collider.extents.x;
                        break;
                    }
                }
            }

            position.y += velocity.y * delta;

            min_bounds.y = position.y + collider.center.y - collider.extents.y;
            max_bounds.y = position.y + collider.center.y + collider.extents.y;

            start_tile = {
                static_cast<int>(min_bounds.x + EPSILON) / TileMap::TILE_SIZE,
                static_cast<int>(min_bounds.y) / TileMap::TILE_SIZE
            };
            end_tile = {
                static_cast<int>(max_bounds.x - EPSILON) / TileMap::TILE_SIZE,
                static_cast<int>(max_bounds.y) / TileMap::TILE_SIZE
            };

            for (int y = start_tile.y; y <= end_tile.y; ++y) {
                for (int x = start_tile.x; x <= end_tile.x; ++x) {
                    if (tile_map.get_tile({x, y}) > 0) {
                        if (velocity.y > 0) {
                            position.y = static_cast<float>(y) * TileMap::TILE_SIZE - collider.extents.y - collider.
                                         center.
                                         y;
                            on_floor = true;
                        } else if (velocity.y < 0) {
                            position.y = static_cast<float>(y + 1) * TileMap::TILE_SIZE + collider.extents.y - collider.
                                         center.y;
                            on_ceiling = true;
                        }
                        velocity.y = 0;
                        break;
                    }
                }
            }
        }

        AABBCollider collider{{}};
        glm::vec2 velocity{};

    private:
        bool on_floor{};
        bool on_ceiling{};
        bool on_wall{};
    };
}
