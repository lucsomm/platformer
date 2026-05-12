#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <new>

namespace platformer {
    class StateMachineBase;

    class State {
    public:
        friend StateMachineBase;

        State() = default;

        virtual ~State() = default;

        State(const State&) = default;

        State& operator=(const State& other) = default;

        State(State&&) = default;

        State& operator=(State&&) = default;

        virtual void enter();

        virtual void update(float delta);

        virtual void physics_update(float delta);

        virtual void draw();

        virtual void exit();

        [[nodiscard]] const StateMachineBase& get_state_machine() const {
            assert(state_machine != nullptr);
            return *state_machine;
        }

        [[nodiscard]] StateMachineBase& get_state_machine() {
            assert(state_machine != nullptr);
            return *state_machine;
        }

    private:
        StateMachineBase* state_machine{};
    };

    namespace concepts {
        template<typename T>
        concept State = std::derived_from<T, platformer::State>;
    }

    class StateMachineBase {
    public:
        explicit StateMachineBase(State* buffer_ptr) : current_state(buffer_ptr) {
            create_state<State>();
        }

        ~StateMachineBase() {
            destroy_state();
        }

        template<concepts::State T, typename... Args>
        void change_state(Args&&... args) {
            destroy_state();
            create_state<T>(std::forward<Args>(args)...);
        }

    private:
        template<concepts::State T, typename... Args>
        void create_state(Args&&... args) {
            new(current_state) T(std::forward<Args>(args)...);
            current_state->state_machine = this;
            current_state->enter();
        }

        void destroy_state() const {
            current_state->exit();
            current_state->~State();
        }

        State* const current_state{};
    };

    namespace concepts {
        template<typename T>
        concept StateMachine = std::derived_from<T, StateMachineBase>;
    }

    template<concepts::State... States>
    class StateMachine : public StateMachineBase {
    public:
        static constexpr size_t STATE_BUFFER_SIZE{std::max({sizeof(State), sizeof(States)...})};
        static constexpr size_t STATE_BUFFER_ALIGN{std::max({alignof(State), alignof(States)...})};

        StateMachine() : StateMachineBase(reinterpret_cast<State*>(state_buffer.data())) {
        }

    private:
        alignas(STATE_BUFFER_ALIGN) std::array<std::byte, STATE_BUFFER_SIZE> state_buffer;
    };
}
