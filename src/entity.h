#pragma once
#include <vector>

#include "glm/vec2.hpp"
#include "transform.h"

namespace platformer {
    class EntityBase {
    public:
        using TypeId = size_t;
        using UpdateEntitiesFunc = void (*)(float delta);
        using PhysicsUpdateEntitiesFunc = void (*)(float delta);
        using DrawEntitiesFunc = void (*)(float alpha);

        explicit EntityBase(const TypeId id) : id{id} {
        }

        [[nodiscard]] TypeId get_id() const {
            return id;
        }

        static void update_all(const float delta) {
            for (auto entity_func: update_funcs) {
                entity_func(delta);
            }
        }

        static void physics_update_all(const float delta) {
            for (auto entity_func: physics_update_funcs) {
                entity_func(delta);
            }
        }

        static void draw_all(const float alpha) {
            for (auto entity_func: draw_funcs) {
                entity_func(alpha);
            }
        }

        glm::vec2 position{};

    protected:
        template<typename>
        static TypeId get_id() {
            static TypeId id = get_next_type_id();
            return id;
        }

        template<typename>
        static void register_type(UpdateEntitiesFunc f1, PhysicsUpdateEntitiesFunc f2, DrawEntitiesFunc f3) {
            update_funcs.push_back(f1);
            physics_update_funcs.push_back(f2);
            draw_funcs.push_back(f3);
        }

        glm::vec2 previous_position{};

    private:
        static TypeId get_next_type_id() {
            static TypeId id_counter = 0;
            return id_counter++;
        }

        static std::vector<UpdateEntitiesFunc> update_funcs;
        static std::vector<PhysicsUpdateEntitiesFunc> physics_update_funcs;
        static std::vector<DrawEntitiesFunc> draw_funcs;
        TypeId id{};
    };

    template<typename Derived>
    class Entity : public EntityBase {
    public:
        template<typename... Args>
        static Derived& create(Args&&... args) {
            static auto register_type = [&] {
                EntityBase::register_type<Derived>(update_entities, physics_update_entities, draw_entities);
            }();
            entities.emplace_back(std::forward<Args>(args)...);
            return entities.back();
        }

        static TypeId get_id() {
            return get_id<Derived>();
        }

        static void reserve(size_t size) {
            entities.reserve(size);
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
        Entity() : EntityBase(get_id<Derived>()) {
        }

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
