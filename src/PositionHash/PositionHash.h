#ifndef CHESS_ENGINE_POSITIONHASH_H
#define CHESS_ENGINE_POSITIONHASH_H

#include "Board/Board.h"
#include "Enums/Pieces.h"
#include "Utils.h"
#include <cstdint>
#include <functional>
#include <optional>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

class PositionHash {

    /**
     * key -> hash
     * value -> evaluation
     */
    std::unordered_map<uint64_t, float> seenPositions;

  public:
    PositionHash() : seenPositions(){};

    struct UpdateHashProps {
        Square toSquare;
        Pieces toPiece;

        Square fromSquare;
        Pieces fromPiece;

        std::optional<Square> captureSquare;
        std::optional<Pieces> capturePiece;

        std::optional<Square> castlesRookFromSquare;
        std::optional<Square> castlesRookToSquare;
        std::optional<Pieces> castlesRookPiece;
    };

  private:
    static std::function<uint64_t()> getGeneratorFunc() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> distrib(0, 1UL << 63);
        return [&distrib, &gen]() -> uint64_t { return distrib(gen); };
    }

    static std::array<std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE>, Utils::NUMBER_PIECE_TYPES * 2> getPieceSquareArray() {
        std::array<std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE>, Utils::NUMBER_PIECE_TYPES * 2> table{};

        for (const uint8_t &piece : std::array<uint8_t, Utils::NUMBER_PIECE_TYPES * 2>{KING_WHITE,
                                                                                       QUEEN_WHITE,
                                                                                       BISHOP_WHITE,
                                                                                       ROOK_WHITE,
                                                                                       KNIGHT_WHITE,
                                                                                       PAWN_WHITE,
                                                                                       KING_BLACK,
                                                                                       QUEEN_BLACK,
                                                                                       BISHOP_BLACK,
                                                                                       ROOK_BLACK,
                                                                                       KNIGHT_BLACK,
                                                                                       PAWN_BLACK}) {

            for (uint8_t i = 0; i < Utils::NUMBER_SQUARES_TABLE; i++) {
                table[piece][i] = generator();
            }
        }
        return table;
    };

    static std::array<uint64_t, Utils::ROW_NUMBER> getEnPassantArray() {
        std::array<uint64_t, Utils::ROW_NUMBER> table{};

        for (auto &value : table) {
            value = generator();
        }
        return table;
    }

    static std::array<uint64_t, 4> getCastleArray() {
        std::array<uint64_t, 4> table{};

        for (auto &value : table) {
            value = generator();
        }
        return table;
    }

    static std::function<uint64_t()> generator;

    std::array<std::array<uint64_t, Utils::NUMBER_SQUARES_TABLE>, Utils::NUMBER_PIECE_TYPES * 2> PIECE_SQUARE_CONSTANTS =
        getPieceSquareArray();

    std::array<uint64_t, Utils::ROW_NUMBER> EN_PASSANT_CONSTANTS = getEnPassantArray();

    std::array<uint64_t, 4> CASTLE_CONSTANTS = getCastleArray();

    std::array<uint64_t, Utils::ROW_NUMBER> DEPTH_CONSTANTS = getEnPassantArray();

  public:
    [[nodiscard]] std::pair<uint64_t, bool> exists(const Board &board, const uint8_t &depth = 0) const noexcept {
        uint64_t hash = 0;
        if (board.turn == WHITE) {
            Utils::runForEachSetBit(board.king.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[KING_WHITE][square];
            });
            Utils::runForEachSetBit(board.queens.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[QUEEN_WHITE][square];
            });
            Utils::runForEachSetBit(board.bishops.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[BISHOP_WHITE][square];
            });
            Utils::runForEachSetBit(board.rooks.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[ROOK_WHITE][square];
            });
            Utils::runForEachSetBit(board.knights.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[KNIGHT_WHITE][square];
            });
            Utils::runForEachSetBit(board.pawns.getBitboard<WHITE>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[PAWN_WHITE][square];
            });
            hash ^= board.castleWhite.hasRookMoved<QUEEN_SIDE>() ? this->CASTLE_CONSTANTS[0] : 0;
            hash ^= board.castleWhite.hasRookMoved<KING_SIDE>() ? this->CASTLE_CONSTANTS[1] : 0;
        } else {
            Utils::runForEachSetBit(board.king.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[KING_BLACK][square];
            });
            Utils::runForEachSetBit(board.queens.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[QUEEN_BLACK][square];
            });
            Utils::runForEachSetBit(board.bishops.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[BISHOP_BLACK][square];
            });
            Utils::runForEachSetBit(board.rooks.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[ROOK_BLACK][square];
            });
            Utils::runForEachSetBit(board.knights.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[KNIGHT_BLACK][square];
            });
            Utils::runForEachSetBit(board.pawns.getBitboard<BLACK>(), [&hash, this](const Square &square) noexcept -> void {
                hash ^= this->PIECE_SQUARE_CONSTANTS[PAWN_BLACK][square];
            });
            // castles
            hash ^= board.castleBlack.hasRookMoved<QUEEN_SIDE>() ? this->CASTLE_CONSTANTS[3] : 0;
            hash ^= board.castleBlack.hasRookMoved<KING_SIDE>() ? this->CASTLE_CONSTANTS[2] : 0;
        }
        // en passant
        hash ^= this->EN_PASSANT_CONSTANTS[static_cast<uint8_t>(Utils::popLSBCopy(board.enPassant)) % 8];
        hash ^= depth == 0 ? 0 : this->DEPTH_CONSTANTS[depth];
        if (board.turn == WHITE)
            hash |= 1;
        else
            hash &= 0xffffffffffffffff ^ 1;

        return {hash, seenPositions.contains(hash)};
    }

    void addHash(const uint64_t &hash, const float &evaluation) { seenPositions[hash] = evaluation; }

    float getEvaluation(const uint64_t &hash) const noexcept { return seenPositions.at(hash); }

    void updateHash(Board &board, const UpdateHashProps &updateProps) const {
        uint64_t hash = board.getHash();

        hash ^= this->PIECE_SQUARE_CONSTANTS[updateProps.fromPiece][updateProps.fromSquare];
        hash ^= this->PIECE_SQUARE_CONSTANTS[updateProps.toPiece][updateProps.toSquare];
        if (updateProps.capturePiece.has_value()) {
            hash ^= this->PIECE_SQUARE_CONSTANTS[updateProps.capturePiece.value()][updateProps.captureSquare.value()];
        }

        if (updateProps.castlesRookFromSquare.has_value()) {
            hash ^= this->PIECE_SQUARE_CONSTANTS[updateProps.castlesRookFromSquare.value()][updateProps.castlesRookPiece.value()];
        }

        if (updateProps.castlesRookToSquare.has_value()) {
            hash ^= this->PIECE_SQUARE_CONSTANTS[updateProps.castlesRookToSquare.value()][updateProps.castlesRookPiece.value()];
        }

        if (board.turn == WHITE) {
            hash |= 1;
        } else {
            hash &= 0xffffffffffffffff ^ 1;
        }
        board.setHash(hash);
    }
};

#endif // CHESS_ENGINE_POSITIONHASH_H
