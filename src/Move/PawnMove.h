#ifndef CHESS_ENGINE_PAWNMOVE_H
#define CHESS_ENGINE_PAWNMOVE_H

#include "Move.h"

class PawnMove : public Move {
  public:
    explicit PawnMove(const Move &_move) noexcept : Move(_move){};

    void makeMove(Board &board) noexcept override {
        board.resetHalfMoveClock();

        const auto fromSquare = static_cast<const Square>(this->getFrom());
        const auto toSquare = static_cast<const Square>(this->getTo());
        board.resetEnPassant();

        if (board.turn == WHITE) {
            if (isPromotion()) {
                if (isCapture()) {
                    Move::makeCapture<WHITE>(board, toSquare);
                }
                if (isQueenPromotion()) {
                    board.status = board.material.registerPiecePromotion<WHITE, QUEEN_POINTS>(board.status);
                    board.queens.flipSquare<WHITE>(toSquare);

                } else if (isBishopPromotion()) {
                    board.status = board.material.registerPiecePromotion<WHITE, BISHOP_POINTS>(board.status);
                    board.bishops.flipSquare<WHITE>(toSquare);

                } else if (isRookPromotion()) {
                    board.status = board.material.registerPiecePromotion<WHITE, ROOK_POINTS>(board.status);
                    board.rooks.flipSquare<WHITE>(toSquare);

                } else if (isKnightPromotion()) {
                    board.status = board.material.registerPiecePromotion<WHITE, KNIGHT_POINTS>(board.status);
                    board.knights.flipSquare<WHITE>(toSquare);
                }

                board.pawns.flipSquare<WHITE>(fromSquare);
                Move::makeMove(board);
                return;
            }

            if (isEnPassant()) {
                board.pawns.flipSquare<BLACK>(Utils::popLSBCopy(Utils::setSquare(toSquare) >> Utils::ROW_NUMBER));
                board.pawns.flipSquare<WHITE>(fromSquare);
                board.pawns.flipSquare<WHITE>(toSquare);
                Move::makeMove(board);
                return;
            }
            if (this->isDoublePush()) {
                board.setEnPassant(Utils::setSquare(toSquare) >> Utils::ROW_NUMBER);
                board.pawns.flipSquare<WHITE>(fromSquare);
                board.pawns.flipSquare<WHITE>(toSquare);
                Move::makeMove(board);
                return;
            }
            if (isCapture()) {
                Move::makeCapture<WHITE>(board, toSquare);
            }

            board.pawns.flipSquare<WHITE>(fromSquare);
            board.pawns.flipSquare<WHITE>(toSquare);
            Move::makeMove(board);
        } else {
            if (isPromotion()) {
                if (isCapture()) {
                    Move::makeCapture<BLACK>(board, toSquare);
                }
                if (isQueenPromotion()) {
                    board.status = board.material.registerPiecePromotion<BLACK, QUEEN_POINTS>(board.status);
                    board.queens.flipSquare<BLACK>(toSquare);
                } else if (isBishopPromotion()) {
                    board.status = board.material.registerPiecePromotion<BLACK, BISHOP_POINTS>(board.status);
                    board.bishops.flipSquare<BLACK>(toSquare);
                } else if (isRookPromotion()) {
                    board.status = board.material.registerPiecePromotion<BLACK, ROOK_POINTS>(board.status);
                    board.rooks.flipSquare<BLACK>(toSquare);
                } else if (isKnightPromotion()) {
                    board.status = board.material.registerPiecePromotion<BLACK, KNIGHT_POINTS>(board.status);
                    board.knights.flipSquare<BLACK>(toSquare);
                }

                board.pawns.flipSquare<BLACK>(fromSquare);
                Move::makeMove(board);
                return;
            }
            if (isEnPassant()) {
                board.pawns.flipSquare<WHITE>(Utils::popLSBCopy(Utils::setSquare(toSquare) << Utils::ROW_NUMBER));
                board.pawns.flipSquare<BLACK>(fromSquare);
                board.pawns.flipSquare<BLACK>(toSquare);
                Move::makeMove(board);
                return;
            }
            if (this->isDoublePush()) {
                board.setEnPassant(Utils::setSquare(toSquare) << Utils::ROW_NUMBER);
                board.pawns.flipSquare<BLACK>(fromSquare);
                board.pawns.flipSquare<BLACK>(toSquare);
                Move::makeMove(board);
                return;
            }
            if (isCapture()) {
                Move::makeCapture<BLACK>(board, toSquare);
            }
            board.pawns.flipSquare<BLACK>(fromSquare);
            board.pawns.flipSquare<BLACK>(toSquare);
            Move::makeMove(board);
        }
    }
};

#endif // CHESS_ENGINE_PAWNMOVE_H
