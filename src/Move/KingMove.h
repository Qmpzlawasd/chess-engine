#ifndef CHESS_ENGINE_KINGMOVE_H
#define CHESS_ENGINE_KINGMOVE_H

#include "Move.h"

class KingMove : public Move {
    virtual void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {

            board.king.flipSquare<WHITE>(fromSquare);
            board.king.flipSquare<WHITE>(toSquare);
            board.castleWhite.kingMoved();
            if (isQueenSideCastle()) {
                board.rooks.flipSquare<WHITE>(A1);
                board.rooks.flipSquare<WHITE>(D1);
            } else if (isKingSideCastle()) {
                board.rooks.flipSquare<WHITE>(H1);
                board.rooks.flipSquare<WHITE>(F1);
            } else {
                Move::makeCapture<WHITE>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
            }

        } else {
            board.king.flipSquare<BLACK>(fromSquare);
            board.king.flipSquare<BLACK>(toSquare);
            board.castleBlack.kingMoved();
            if (isQueenSideCastle()) {
                board.rooks.flipSquare<WHITE>(A8);
                board.rooks.flipSquare<WHITE>(D8);
            } else if (isKingSideCastle()) {
                board.rooks.flipSquare<WHITE>(H8);
                board.rooks.flipSquare<WHITE>(F8);
            } else {
                Move::makeCapture<BLACK>(board, Utils::setSquare(toSquare) & board.getOccupiedSquares<BLACK>());
            }
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_KINGMOVE_H
