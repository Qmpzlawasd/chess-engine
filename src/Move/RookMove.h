#ifndef CHESS_ENGINE_ROOKMOVE_H
#define CHESS_ENGINE_ROOKMOVE_H

#include "Move.h"

class RookMove : public Move {
  public:
    RookMove(const Move &move) : Move(move){};
    virtual void makeMove(Board &board) noexcept override {
        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());

        if (board.turn == WHITE) {
            Move::makeCapture<WHITE>(board, toSquare);

            if (fromSquare == Utils::KING_SIDE_CASTLE_WHITE) {
                board.castleWhite.rookMoved<KING_SIDE>();
            } else if (fromSquare == Utils::QUEEN_SIDE_CASTLE_WHITE) {
                board.castleWhite.rookMoved<QUEEN_SIDE>();
            }

            board.rooks.flipSquare<WHITE>(fromSquare);
            board.rooks.flipSquare<WHITE>(toSquare);

        } else {
            Move::makeCapture<BLACK>(board, toSquare);

            if (fromSquare == Utils::KING_SIDE_CASTLE_BLACK) {
                board.castleBlack.rookMoved<KING_SIDE>();
            } else if (fromSquare == Utils::QUEEN_SIDE_CASTLE_BLACK) {
                board.castleBlack.rookMoved<QUEEN_SIDE>();
            }

            board.rooks.flipSquare<BLACK>(fromSquare);
            board.rooks.flipSquare<BLACK>(toSquare);
        }

        board.resetEnPassant();
        Move::makeMove(board);
    }
};

#endif // CHESS_ENGINE_ROOKMOVE_H
