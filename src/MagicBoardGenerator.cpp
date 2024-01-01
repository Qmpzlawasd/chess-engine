#pragma once
#include <thread>
#include <unordered_map>
#include <vector>
#include "Utils.cpp"
#include "Squares.cpp"

class MagicBoardGenerator
{
public:
    virtual const std::vector<uint64_t> generate() const noexcept = 0;
    virtual const std::vector<std::vector<uint64_t>> getTables() const noexcept = 0;
    virtual ~MagicBoardGenerator() = default;
};
