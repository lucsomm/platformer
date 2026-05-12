#pragma once
#include <algorithm>
#include <array>
#include <concepts>

namespace platformer {
    class State;

    class StateMachineBase {
    public:
        explicit StateMachineBase(State* buffer_ptr) : buffer_ptr(buffer_ptr) {
        }

    private:
        State* buffer_ptr{};
    };

    namespace concepts {
        template<typename T>
        concept StateMachine = std::derived_from<T, StateMachineBase>;
    }

    class State {
    public:
        template<concepts::StateMachine T>
        explicit State(T& state_machine) : state_machine{state_machine} {
        }

        virtual ~State() = default;

        State(const State&) = delete;

        State& operator=(const State& other) = delete;

        State(State&&) = delete;

        State& operator=(State&&) = delete;

        virtual void enter();

        virtual void update(float delta);

        virtual void physics_update(float delta);

        virtual void draw();

        virtual void exit();

    protected:
        StateMachineBase& state_machine;
    };

    namespace concepts {
        template<typename T>
        concept State = std::derived_from<T, platformer::State>;
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
