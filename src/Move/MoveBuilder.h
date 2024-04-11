#ifndef CHESS_ENGINE_MOVEBUILDER_H
#define CHESS_ENGINE_MOVEBUILDER_H

#include "Enums/Squares.h"
#include "Move.h"

class MoveBuilder {
    Move move;

  public:
    explicit MoveBuilder() : move(0){};

    MoveBuilder &toSquare(const Square &);

    MoveBuilder &fromSquare(const Square &);

    MoveBuilder &withCapture();

    Move withKnightPromotion();

    Move withKnightPromotionCapture();

    Move withBishopPromotion();

    Move withBishopPromotionCapture();

    Move withRookPromotion();

    Move withRookPromotionCapture();

    Move withQueenPromotion();

    Move withQueenPromotionCapture();

    Move withDoublePawnPush();

    Move withKingSideCastle();

    Move withQueenSideCastle();

    Move withEnPassantCapture();

    Move getMove();
};

#endif // CHESS_ENGINE_MOVEBUILDER_H
