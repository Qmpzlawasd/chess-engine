#ifndef CHESS_ENGINE_ROOKMOVE_H
#define CHESS_ENGINE_ROOKMOVE_H

#include "Move.h"

class RookMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const Square fromSquare = static_cast<const Square>(this->getFrom());
        const Square toSquare = static_cast<const Square>(this->getTo());
        if (board.turn == WHITE) {
            board.rooks.flipSquare<WHITE>(fromSquare);
            board.rooks.flipSquare<WHITE>(toSquare);

        } else {
            board.rooks.flipSquare<BLACK>(fromSquare);
            board.rooks.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_ROOKMOVE_H
