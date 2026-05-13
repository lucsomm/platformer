#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <new>
#include <type_traits>

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

        virtual void enter() {
        }

        virtual void update(float delta) {
        }

        virtual void physics_update(float delta) {
        }

        virtual void draw() {
        }

        virtual void exit() {
        }

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

    template<concepts::State DefaultState, concepts::State... States>
    class StateMachine;

    class StateMachineBase {
    public:
        template<concepts::State DefaultState, concepts::State... States>
        friend class StateMachine;

        virtual ~StateMachineBase() {
            destroy_state();
        }

        StateMachineBase(const StateMachineBase&) = delete;

        StateMachineBase& operator=(const StateMachineBase&) = delete;

        StateMachineBase(StateMachineBase&&) = delete;

        StateMachineBase& operator=(StateMachineBase&&) = delete;

        [[nodiscard]] const State& get_current_state() const {
            return *current_state;
        }

        [[nodiscard]] State& get_current_state() {
            return *current_state;
        }

        template<concepts::State T, typename... Args>
        void change_state(Args&&... args) {
            destroy_state();
            create_state<T>(std::forward<Args>(args)...);
        }

    private:
        explicit
        StateMachineBase(State* buffer_ptr, const size_t buffer_size,
                         const size_t buffer_align) : current_state(buffer_ptr),
                                                      buffer_size{buffer_size},
                                                      buffer_align{buffer_align} {
        }

        template<concepts::State T, typename... Args>
        void create_state(Args&&... args) {
            assert(sizeof(T) <= buffer_size);
            assert(alignof(T) <= buffer_align);

            current_state = new(current_state) T(std::forward<Args>(args)...);
            current_state->state_machine = this;
            current_state->enter();
        }

        void destroy_state() const {
            current_state->exit();
            std::destroy_at(current_state);
        }

        State* current_state{};
        size_t buffer_size{};
        size_t buffer_align{};
    };

    namespace concepts {
        template<typename T>
        concept StateMachine = std::derived_from<T, StateMachineBase>;
    }

    template<concepts::State DefaultState = State, concepts::State... States>
    class StateMachine final : public StateMachineBase {
    public:
        static constexpr size_t STATE_BUFFER_SIZE{std::max({sizeof(DefaultState), sizeof(States)...})};
        static constexpr size_t STATE_BUFFER_ALIGN{std::max({alignof(DefaultState), alignof(States)...})};

        template<typename... Args>
        explicit StateMachine(Args... args) : StateMachineBase(reinterpret_cast<State*>(state_buffer.data()),
                                                               STATE_BUFFER_SIZE,
                                                               STATE_BUFFER_ALIGN) {
            create_state<DefaultState>(std::forward<Args>(args)...);
        }

        template<concepts::State InitialState, typename... Args>
        explicit StateMachine(std::in_place_type_t<InitialState>, Args... args) : StateMachineBase(
            reinterpret_cast<State*>(state_buffer.data()),
            STATE_BUFFER_SIZE,
            STATE_BUFFER_ALIGN) {
            create_state<InitialState>(std::forward<Args>(args)...);
        }

    private:
        alignas(STATE_BUFFER_ALIGN) std::array<std::byte, STATE_BUFFER_SIZE> state_buffer;
    };
}
