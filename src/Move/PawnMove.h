#ifndef CHESS_ENGINE_PAWNMOVE_H
#define CHESS_ENGINE_PAWNMOVE_H

#include "Move.h"

class PawnMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());
        if (board.turn == WHITE) {
            board.pawns.flipSquare<WHITE>(fromSquare);
            board.pawns.flipSquare<WHITE>(toSquare);
            if (isCapture()) {
                Move::makeCapture<BLACK>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
            }
        } else {
            board.pawns.flipSquare<BLACK>(fromSquare);
            board.pawns.flipSquare<BLACK>(toSquare);
        }
        if (this->isDoublePush()) {
            board.setEnPassant(Utils::setSquare(fromSquare) << Utils::ROW_NUMBER);
        } else {
            board.resetEnPassant();
        }

        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_PAWNMOVE_H
