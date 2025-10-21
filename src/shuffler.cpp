#include "shuffler.hpp"
#include <algorithm>
#include <ranges>
#include <random>


auto shuffler::shuffle (const std::string_view entries) -> std::vector<std::string>
{
    static std::mt19937 mt (std::random_device{}());
    auto entries_splitted = entries |
                            std::views::split ('\n') | 
                            std::views::filter([](const auto &e){return e.size()>1;}) | 
                            std::ranges::to<std::vector<std::string>>();
    std::ranges::shuffle(entries_splitted,mt);
    return entries_splitted;
}
