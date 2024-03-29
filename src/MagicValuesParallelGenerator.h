#ifndef CHESS_ENGINE_MAGICVALUESPARALLELGENERATOR_H
#define CHESS_ENGINE_MAGICVALUESPARALLELGENERATOR_H

#include "Enums/Squares.h"
#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "Piece/Piece.h"
#include "Piece/SlidingPiece.h"

#include <functional>
#include <thread>
#include <unordered_map>
#include <vector>
#include <cassert>

class MagicValuesParallelGenerator : public MagicValuesGeneratorInterface {

  private:
    bool tryMagicNumber(const uint64_t &magicNumber,
                        const Square &square,
                        const SlidingPiece &slidingPiece,
                        const std::function<bool(uint64_t, uint64_t)> &function) const {
        uint64_t subset = 0;
        do {
            __uint128_t partialKey = subset * magicNumber;
            const uint64_t key = partialKey >> slidingPiece.getShiftValue();
            if (!function(key, subset)) {
                return false;
            }

            subset = slidingPiece.getNaiveAttackPattern(square) & ((subset | ~slidingPiece.getNaiveAttackPattern(square)) + 1);

        } while (subset != 0);
        return true;
    }

    void getMagicNumber(const Square &square, const SlidingPiece &slidingPiece) const {
        std::unordered_map<uint64_t, uint64_t> magic;
        for (uint64_t MAGIC_NUMBER = 43004830050835876; MAGIC_NUMBER < 1844670000000000000; MAGIC_NUMBER += 1231823786127) {
            magic.clear();
            if (tryMagicNumber(MAGIC_NUMBER, square, slidingPiece, [&magic](const uint64_t &key, const uint64_t &subset) -> bool {
                    if (magic.contains(key)) {
                        return false;
                    }

                    magic[key] = 1;
                    return true;
                })) {
                printf("Magic number %ld found for square %d\n", MAGIC_NUMBER, square);
                return;
            }
        }
        printf("Magic number not found for square %d\n", square);
    };

    void startSearchThreads(const SlidingPiece &slidingPiece) const {
        constexpr uint8_t start = 0;

        std::vector<std::thread> searchThreads;
        for (uint8_t i = start; i < Utils::NUMBER_SQUARES_TABLE; i++) {
            searchThreads.emplace_back(&MagicValuesParallelGenerator::getMagicNumber,
                                       MagicValuesParallelGenerator{},
                                       static_cast<Square>(i),
                                       std::ref(slidingPiece));
        }
        std::for_each(searchThreads.begin(), searchThreads.end(), [](std::thread &thread) -> void { thread.join(); });
    }

  public:
    MagicValuesParallelGenerator() = default;

    void generate(const SlidingPiece &slidingPiece) const noexcept override { startSearchThreads(slidingPiece); }

    [[nodiscard]] std::vector<std::vector<uint64_t>> getTables(const std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE> &magicValues,
                                                               const SlidingPiece &slidingPiece) const noexcept override {

        const uint64_t tableSize = 1 << (Utils::NUMBER_SQUARES_TABLE - slidingPiece.getShiftValue());
        std::vector<std::vector<uint64_t>> bitboard(Utils::NUMBER_SQUARES_TABLE, std::vector<uint64_t>(tableSize, 123456789));

        for (uint8_t square = 0; square < Utils::NUMBER_SQUARES_TABLE; square++) {
            tryMagicNumber(magicValues[square],
                           static_cast<Square>(square),
                           slidingPiece,
                           [&bitboard, &square, &slidingPiece](const uint64_t &key, const uint64_t &subset) noexcept -> bool {
                               assert(bitboard[square][key] == 123456789);
                               bitboard[square][key] = slidingPiece.getBlockedAttackPattern(static_cast<const Square>(square), subset);
                               return true;
                           });
        }
        return bitboard;
    }
};

#endif // CHESS_ENGINE_MAGICVALUESPARALLELGENERATOR_H