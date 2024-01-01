#pragma once
#include <thread>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "Utils.cpp"
#include "Squares.cpp"
#include "MagicBoardGenerator.cpp"

class ParallelGenerator : public MagicBoardGenerator
{
private:
    static const bool tryMagicNumber(const uint64_t &magicNumber, const uint64_t &pattern)
    {
        std::unordered_map<uint64_t, uint64_t> magic;
        uint64_t max = 0;
        uint64_t subset = 0;
        do
        {
            __uint128_t asd = subset * magicNumber;
            const uint64_t key = asd >> 48;

            // std::cout << key << '\n';

            if (key > max)
            {
                max = key;
            }
            if (magic.contains(key))
            {
                // std::cout << "RIP "<< "\n";
                // Utils::showBitBoard(subset);
                // printf("Min: %ld, Max: %ld\n", magic.size(), max);

                return false;
            }

            magic[key] = subset;
            subset = pattern & ((subset | ~pattern) + 1);

        } while (subset != 0);
        printf("Max: %ld\n", max);
        return true;
    }

    static void threadStart(const uint8_t &count)
    {
        // std::vector<std::thread> searchThreads;
        // for (uint8_t i = 29; i < count; i++)
        // {
        //     searchThreads.push_back(std::thread(getMagicNumber, (Square)i));
        // }
        // for (uint8_t i = 29; i < count; i++)
        // {
        //     searchThreads[i].join();
        // }
        getMagicNumber((Square)29);
    }

    static void getMagicNumber(const Square& square)
    {
        const uint8_t line = square % Utils::ROW_NUMBER;
        const uint8_t column = square / Utils::ROW_NUMBER;

        const uint64_t PIECE_FILE = Utils::A_FILE << line;
        const uint64_t PIECE_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * column;

        const uint64_t naiveAttackPattern = (PIECE_FILE | PIECE_ROW) & ~Utils::setSquare(Square(square));

            // std::cout <<"am iseit" << "\n";
        for (uint64_t MAGIC_NUMBER = 865473128378; MAGIC_NUMBER < 100000000000000; MAGIC_NUMBER += 23123343)
        {

            // std::cout << 1000000000000000 - MAGIC_NUMBER << "\n";
            if (tryMagicNumber(MAGIC_NUMBER, naiveAttackPattern))
            {
                printf("Magic number %ld found for square %d\n", MAGIC_NUMBER, square);
                return;
            }
        }

    };

public:
    const std::vector<uint64_t> generate() const noexcept override
    {
        threadStart(30);
        return std::vector<uint64_t>{};
    }
    const std::vector<std::vector<uint64_t>> getTables() const noexcept override
    {
        return std::vector<std::vector<uint64_t>>{};
    }
};
