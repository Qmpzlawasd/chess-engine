#pragma once
#include "MagicBoardGenerator.cpp"
#include "MagicValues.cpp"
#include "Piece.cpp"
#include "Squares.cpp"
#include "Utils.cpp"
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

class ParallelGenerator : public MagicBoardGenerator {
  private:
    static const void tryMagicNumber(const uint64_t &magicNumber,
                                     const uint64_t &pattern,
                                     std::function<bool(uint64_t, uint64_t)> function) {
        uint64_t subset = 0;
        do {
            __uint128_t asd = subset * magicNumber;
            const uint64_t key = asd >> 48;
            if (!function(key, subset)) {
                return;
            }

            subset = pattern & ((subset | ~pattern) + 1);

        } while (subset != 0);
    }

    static void threadStart(const uint8_t &count) {
        constexpr uint8_t start = 0;
        std::vector<std::thread> searchThreads;
        for (uint8_t i = start; i < count; i++) {
            searchThreads.push_back(std::thread(getMagicNumber, (Square)i));
        }
        for (uint8_t i = start; i < count; i++) {
            searchThreads[i].join();
        }
        // getMagicNumber(Square(27));
    }

    static void getMagicNumber(const Square &square) {
        const uint64_t naiveAttackPattern = Bishop::getNaiveAttackPattern(square);
        std::unordered_map<uint64_t, uint64_t> magic;
        for (uint64_t MAGIC_NUMBER = 1389071912382857223; MAGIC_NUMBER < 1844670000000000000;
             MAGIC_NUMBER += 43423421323423) {
            magic.clear();
            tryMagicNumber(MAGIC_NUMBER,
                           naiveAttackPattern,
                           [&magic](const uint64_t &key, const uint64_t &subset) {
                               if (magic.contains(key)) {
                                   return false;
                               }
                               magic[key] = subset;
                               return true;
                           });
            if (magic.size() == 64) {
                printf("Magic number %ld found for square %d\n", MAGIC_NUMBER, square);
                return;
            }
        }
        printf("Magic number not found for square %d\n", square);
    };

  public:
    const std::vector<uint64_t> generate() const noexcept override {
        threadStart(64);
        return std::vector<uint64_t>{};
    }

    const std::vector<std::vector<uint64_t>>
    getTables(const uint64_t &size,
              const uint64_t ROOK_MAGIC_VALS[],
              const uint8_t &rookShift) const noexcept override {

        std::vector<std::vector<uint64_t>> bitboard(64, std::vector<uint64_t>(size, -1));

        for (uint8_t i = 0; i < 64; i++) {
            tryMagicNumber(ROOK_MAGIC_VALS[i],
                           Rook::getNaiveAttackPattern((Square)i),
                           [&bitboard, &i](const uint64_t &key, const uint64_t &subset) {
                               if (bitboard[i][key] != -1) {
                                   puts("PANIC");
                                   return false;
                               }

                               bitboard[i][key] = subset;
                               return true;
                           });
        }
        return bitboard;
    }
};
