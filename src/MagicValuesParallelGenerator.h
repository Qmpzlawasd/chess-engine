#pragma once

#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "Piece.h"
#include "Squares.h"
#include <array>
#include <bitset>
#include <functional>
#include <thread>
#include <unordered_map>
#include <vector>

class MagicValuesParallelGenerator : public MagicValuesGeneratorInterface {

  private:
    void tryMagicNumber(const uint64_t &magicNumber, const std::function<bool(uint64_t, uint64_t)> &function) const {
        uint64_t subset = 0;
        do {
            __uint128_t partialKey = subset * magicNumber;
            const uint64_t key = partialKey >> shiftValue;
            if (!function(key, subset)) {
                return;
            }

            subset = naiveAttackPattern & ((subset | ~naiveAttackPattern) + 1);

        } while (subset != 0);
    }

    void startSearchThreads() const {
        constexpr uint8_t start = 0;

        std::vector<std::thread> searchThreads;
        for (uint8_t i = start; i < Utils::NUMBER_SQUARES_TABLE; i++) {
            if (i == 25 or i == 8 or i == 34 or i == 26 or i == 40 or i == 43 or i == 50) {

                searchThreads.emplace_back(&MagicValuesParallelGenerator::getMagicNumber,
                                           MagicValuesParallelGenerator{shiftValue, naiveAttackPattern},
                                           (Square)i);
            }
        }
        for (uint8_t i = start; i < Utils::NUMBER_SQUARES_TABLE; i++) {
            if (i == 25 or i == 8 or i == 34 or i == 26 or i == 40 or i == 43 or i == 50) {
                searchThreads[i].join();
            }
        }
    }

    void getMagicNumber(const Square &square) const {

        std::unordered_map<uint64_t, uint64_t> magic;
        for (uint64_t MAGIC_NUMBER = 8947083712312; MAGIC_NUMBER < 1844670000000000000; MAGIC_NUMBER += 1231823786127) {
            magic.clear();
            tryMagicNumber(MAGIC_NUMBER, [&magic](const uint64_t &key, const uint64_t &subset) -> bool {
                if (magic.contains(key)) {
                    return false;
                }
                magic[key] = subset;
                return true;
            });
            if (magic.size() == Utils::COLUMN_NUMBER * Utils::ROW_NUMBER) {
                printf("Magic number %ld found for square %d\n", MAGIC_NUMBER, square);
                return;
            }
        }
        printf("Magic number not found for square %d\n", square);
    };

  public:
    explicit MagicValuesParallelGenerator(const uint8_t &shiftValue, const uint64_t &naiveAttackPattern = 0)
        : MagicValuesGeneratorInterface(shiftValue, naiveAttackPattern) {}

    void generate() const noexcept override { startSearchThreads(); }

    [[nodiscard]] std::vector<std::vector<uint64_t>> getTables(const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> magicValues,
                                                               const SlidingPiece &slidingPiece) const noexcept override {

        const uint64_t tableSize = 1 << (Utils::NUMBER_SQUARES_TABLE - shiftValue);
        std::vector<std::vector<uint64_t>> bitboard(Utils::NUMBER_SQUARES_TABLE, std::vector<uint64_t>(tableSize, -1));

        for (uint8_t square = 0; square < Utils::NUMBER_SQUARES_TABLE; square++) {
            tryMagicNumber(magicValues[square], [&bitboard, &square, &slidingPiece](const uint64_t &key, const uint64_t &subset) -> bool {
                if (bitboard[square][key] != (uint64_t)-1) {
                    puts("PANIC getTables");
                    std::cout << (int)square << '\n';
                    return false;
                }

                bitboard[square][key] = slidingPiece.fillPositions((const Square)square, subset);
                return true;
            });
        }
        return bitboard;
    }
};
