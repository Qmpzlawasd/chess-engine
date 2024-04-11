#include "MoveBuilder.h"
#include "Move.h"

MoveBuilder &MoveBuilder::toSquare(const Square &toSquare) {
    move.setToSquare(toSquare);
    return *this;
}

MoveBuilder &MoveBuilder::fromSquare(const Square &fromSquare) {
    move.setFromSquare(fromSquare);
    return *this;
}

MoveBuilder &MoveBuilder::withCapture() {
    move.setCapture();
    return *this;
}

Move MoveBuilder::withDoublePawnPush() {
    move.setDoublePawnPush();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withKingSideCastle() {
    move.setKingSideCastle();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withQueenSideCastle() {
    move.setQueenSideCastle();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withEnPassantCapture() {
    move.setEnPassantCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withKnightPromotionCapture() {
    move.setKnightPromotionCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withBishopPromotionCapture() {
    move.setBishopPromotionCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withRookPromotionCapture() {
    move.setRookPromotionCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::withQueenPromotionCapture() {
    move.setQueenPromotionCapture();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withKnightPromotion() {
    move.setKnightPromotion();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withBishopPromotion() {
    move.setBishopPromotion();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withRookPromotion() {
    move.setRookPromotion();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
Move MoveBuilder::withQueenPromotion() {
    move.setQueenPromotion();
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}

Move MoveBuilder::getMove() {
    Move moveCopy = move.getMoveCopy();
    move.resetMove();
    return moveCopy;
}
