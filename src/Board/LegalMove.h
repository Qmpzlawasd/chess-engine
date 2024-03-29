#ifndef CHESS_ENGINE_LEGALMOVE_H
#define CHESS_ENGINE_LEGALMOVE_H

#include "../Piece/JumpingPiece.h"
#include "Board.h"
#include "Enums/Colors.h"

#include <cstdint>

class LegalMove {
    Board board;

  public:
    explicit LegalMove(const Board &_board) noexcept : board{_board} {}

    template <Color side>
    [[nodiscard]] std::vector<Move> getBishopLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getRookLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getPawnLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKnightLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getQueenLegalMove() const noexcept;

    template <Color side>
    [[nodiscard]] std::vector<Move> getKingLegalMove() const noexcept;
};

#endif // CHESS_ENGINE_LEGALMOVE_H
