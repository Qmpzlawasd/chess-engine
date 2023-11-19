#pragma once
#include <unordered_map>
#include "Utils.cpp"
#include "Piece.cpp"

class MagicBitboard
{
    static constexpr uint64_t NONE = 0xffffffffffffffff;

    // static const bool checkCollision(const uint64_t &key, const std::unordered_map<uint64_t, uint64_t>& magic )
    // {
    //     return magic.contains(key);
    // }

    static const uint64_t generateGoodPattern(const uint64_t &blockers)
    {
        return 0;
    }

public:
    template <class KeyType>
    static const std::unordered_map<KeyType, uint64_t> generateRookPattern(const uint64_t &pattern)
    {

        std::unordered_map<KeyType, uint64_t> magic;

        uint64_t subset = 0;
        for (KeyType MAGIC_NUMBER = 123434; MAGIC_NUMBER < 4294967296; MAGIC_NUMBER += 765)
        {
            magic.clear();
            while (true)
            {

                magic[subset] = generateGoodPattern(subset) * MAGIC_NUMBER >> sizeof(KeyType);
                subset = pattern & (subset | ~pattern) + 1;
                if (subset == 0 or magic.contains(subset))
                {
                    break;
                }
            }
        }

        return magic;
    };
};