#ifndef CHESS_ENGINE_KNIGHTMOVE_H
#define CHESS_ENGINE_KNIGHTMOVE_H

#include "Move.h"

class KnightMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const Square fromSquare = static_cast<const Square>(this->getFrom());
        const Square toSquare = static_cast<const Square>(this->getTo());
        if (board.turn == WHITE) {
            board.knights.flipSquare<WHITE>(fromSquare);
            board.knights.flipSquare<WHITE>(toSquare);
            Move::makeCapture<WHITE>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());

        } else {
            board.knights.flipSquare<BLACK>(fromSquare);
            board.knights.flipSquare<BLACK>(toSquare);
            Move::makeCapture<BLACK>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
        }
        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_KNIGHTMOVE_H
