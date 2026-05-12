#pragma once
#include <algorithm>
#include <concepts>

namespace platformer {
    class State {
    };

    namespace concepts {
        template<typename T>
        concept State = std::derived_from<T, platformer::State>;
    }

    template<typename T, concepts::State... States>
    class StateMachine {
        static constexpr size_t STATE_BUFFER_SIZE{std::max({std::size_t{0}, sizeof(States)...})};
        static constexpr size_t STATE_BUFFER_ALIGN{std::max({std::size_t{0}, alignof(States)...})};
    };
}
