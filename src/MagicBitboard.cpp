#pragma once
#include "Utils.cpp"
#include "Piece.cpp"

class MagicBitboard
{
private:
    static constexpr uint64_t NONE = 0xffffffffffffffff;

    static const bool tryMagicNumber(std::unordered_map<uint64_t, uint64_t> &magic, const uint64_t magicNumber, const uint64_t &pattern)
    {
        uint64_t max = 0;
        uint64_t subset = 0;
        do
        {   
            __uint128_t asd =  subset * magicNumber;
            const uint64_t key = asd >> 48;
            // std::cout << key << '\n';
            if (key > max)
            {
                max = key;
            }
            if (magic.contains(key))
            {
                // std::cout << "RIP "
                //   << "\n";
                // Utils::showBitBoard(subset);
                // printf("Min: %ld, Max: %ld\n", magic.size(), max);

                return false;
            }

            magic[key] = subset;
            subset = pattern & (subset | ~pattern) + 1;

        } while (subset != 0);
        printf("Min: %ld, Max: %ld\n", magic.size(), max);
        return true;
    }

public:
    static const bool generateRookPattern(std::vector<std::unordered_map<uint64_t, uint64_t>> &magic, const uint64_t &pattern)
    {
        uint8_t square = 0;
        for (uint8_t square = 21; square < Utils::COLUMN_NUMBER * Utils::ROW_NUMBER; square++)
        {
            const uint8_t line = square % Utils::ROW_NUMBER;
            const uint8_t column = square / Utils::ROW_NUMBER;

            const uint64_t PIECE_FILE = Utils::A_FILE << line;
            const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

            const uint64_t naiveAttackPattern = (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(Square(square));

            int bine = 0;
            for (uint64_t MAGIC_NUMBER = 18835453908674568; MAGIC_NUMBER < 1000000000000000; MAGIC_NUMBER += 13234322893)
            {
                // std::cout << 91231231222222222 - MAGIC_NUMBER << "\n";
                magic[square].clear();
                if (tryMagicNumber(magic[square], MAGIC_NUMBER, naiveAttackPattern))
                {
                    printf("Found %ld for square %d\n", MAGIC_NUMBER, square);
                    bine = 1;
                    break;
                }
            }
            if (!bine)
            {
                return false;
            }
        }
        return true;
    };
};