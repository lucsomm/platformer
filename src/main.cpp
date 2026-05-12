#include <memory>

#include "transform.h"
#include "game.h"

int main() {
    auto game = std::make_unique<platformer::Game>();
    game->run();
}
