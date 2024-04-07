#ifndef CHESS_ENGINE_QUEENMOVE_H
#define CHESS_ENGINE_QUEENMOVE_H

#include "Move.h"

class QueenMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const Square fromSquare = static_cast<const Square>(this->getFrom());
        const Square toSquare = static_cast<const Square>(this->getTo());
        if (board.turn == WHITE) {
            board.queens.flipSquare<WHITE>(fromSquare);
            Move::makeCapture<WHITE>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
            board.queens.flipSquare<WHITE>(toSquare);

        } else {
            board.queens.flipSquare<BLACK>(fromSquare);
            Move::makeCapture<BLACK>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
            board.queens.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_QUEENMOVE_H
