#include "include/main.hpp"

std::string create_market_snapshot() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    auto val = [&] { return dis(gen); };

    return std::format(
        R"({{"ask":{{"level_1":{},"level_2":{},"level_3":{},"level_4":{},"level_5":{}}},)"
        R"("bid":{{"level_1":{},"level_2":{},"level_3":{},"level_4":{},"level_5":{}}}}})",
        val(), val(), val(), val(), val(), val(), val(), val(), val(), val());
}
