#pragma once
#include <vector>

#include "glm/vec2.hpp"
#include "transform.h"

namespace platformer {
    using EntityId = size_t;

    class EntityBase {
    public:
        using UpdateEntitiesFunc = void (*)(float delta);
        using PhysicsUpdateEntitiesFunc = void (*)(float delta);
        using DrawEntitiesFunc = void (*)(float alpha);

        explicit EntityBase(const EntityId id, const UpdateEntitiesFunc update_entities_func,
                            const PhysicsUpdateEntitiesFunc physics_update_entities_func,
                            const DrawEntitiesFunc draw_entities_func) : id{id} {
            update_entities_funcs.push_back(update_entities_func);
            physics_update_entities_funcs.push_back(physics_update_entities_func);
            draw_entities_funcs.push_back(draw_entities_func);
        }

        [[nodiscard]] EntityId get_id() const {
            return id;
        }

        static void update_all(const float delta) {
            for (auto entity_func: update_entities_funcs) {
                entity_func(delta);
            }
        }

        static void physics_update_all(const float delta) {
            for (auto entity_func: physics_update_entities_funcs) {
                entity_func(delta);
            }
        }

        static void draw_all(const float alpha) {
            for (auto entity_func: draw_entities_funcs) {
                entity_func(alpha);
            }
        }

        glm::vec2 position{};

    protected:
        template<typename>
        static EntityId get_id() {
            static EntityId id = id_counter++;
            return id;
        }

        glm::vec2 previous_position{};

    private:
        static std::vector<UpdateEntitiesFunc> update_entities_funcs;
        static std::vector<PhysicsUpdateEntitiesFunc> physics_update_entities_funcs;
        static std::vector<DrawEntitiesFunc> draw_entities_funcs;
        static inline EntityId id_counter{};
        EntityId id{};
    };

    template<typename Derived>
    class Entity : public EntityBase {
    public:
        Entity() : EntityBase(get_id<Derived>()) {
        }

        void update(float delta) {
            static_cast<Derived*>(this)->update_impl(delta);
        }

        void physics_update(float delta) {
            previous_position = position;
            static_cast<Derived*>(this)->physics_update_impl(delta);
        }

        void draw(const float alpha) {
            glm::vec2 draw_position = previous_position + (position - previous_position) * alpha;
            static_cast<Derived*>(this)->draw_impl(draw_position);
        }

    private:
        static void update_entities(const float delta) {
            for (auto& entity: entities) {
                entity.update(delta);
            }
        }

        static void physics_update_entities(const float delta) {
            for (auto& entity: entities) {
                entity.physics_update(delta);
            }
        }

        static void draw_entities(const float alpha) {
            for (auto& entity: entities) {
                entity.draw(alpha);
            }
        }

        static std::vector<Derived> entities;
    };
}
