#ifndef CHESS_ENGINE_LEGALMOVE_H
#define CHESS_ENGINE_LEGALMOVE_H

#include "../Move/Move.h"
#include "../Piece/JumpingPiece.h"
#include "Board.h"
#include "Enums/Colors.h"
#include <cstdint>
#include <memory>
#include <optional>

class LegalMove {
    Board &board;

  public:
    template <Color side>
    [[nodiscard]] bool handlePawnShenanigans(const Square &) noexcept;

    explicit LegalMove(Board &_board) noexcept : board{_board} {}

    template <Color side>
    [[nodiscard]] std::optional<std::vector<std::shared_ptr<Move>>> getLegalMoves() noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getBishopLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getRookLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getPawnLegalMoves() noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getKnightLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getQueenLegalMoves() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<std::shared_ptr<Move>> getKingLegalMoves() const noexcept;
};

#include "LegalMove.inl"

#endif // CHESS_ENGINE_LEGALMOVE_H
