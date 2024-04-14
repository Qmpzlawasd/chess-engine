#ifndef CHESS_ENGINE_MOVE_H
#define CHESS_ENGINE_MOVE_H

#include "../Board/Board.h"
#include "../Enums/Squares.h"
#include "../Utils.h"

#include <cstdint>
#include <fstream>
#include <vector>

class Move {
    /* 0 -> special 0
     * 1 -> special 1
     * 2 -> capture
     * 3 -> promotion
     * next 6 bits -> FROM square
     * next 6 bits -> TO square
     */
    uint16_t move;

  protected:
    template <Color side>
    void makeCapture(Board &board, const Square &square) noexcept {
        constexpr Color enemy = Utils::flipColor(side);
        const uint64_t capture = Utils::setSquare(square) & board.getOccupiedSquares<enemy>();
        if (!capture)
            return;

        board.resetHalfMoveClock();

        if (board.pawns.getBitboard<enemy>() & capture) {
            board.pawns.flipSquare<enemy>(Utils::popLSBCopy(capture));
            board.status = board.material.registerPieceCapture<side, PAWN_POINTS>(board.status);
        } else if (board.knights.getBitboard<enemy>() & capture) {
            board.knights.flipSquare<enemy>(Utils::popLSBCopy(capture));
            board.status = board.material.registerPieceCapture<side, KNIGHT_POINTS>(board.status);
        } else if (board.bishops.getBitboard<enemy>() & capture) {
            board.bishops.flipSquare<enemy>(Utils::popLSBCopy(capture));
            board.status = board.material.registerPieceCapture<side, BISHOP_POINTS>(board.status);
        } else if (board.rooks.getBitboard<enemy>() & capture) {
            board.rooks.flipSquare<enemy>(Utils::popLSBCopy(capture));
            board.status = board.material.registerPieceCapture<side, ROOK_POINTS>(board.status);
            if constexpr (enemy == WHITE) {
                if (Utils::popLSBCopy(capture) == H1)
                    board.castleWhite.rookMoved<KING_SIDE>();
                else if (Utils::popLSBCopy(capture) == A1)
                    board.castleWhite.rookMoved<QUEEN_SIDE>();
            } else {
                if (Utils::popLSBCopy(capture) == H8)
                    board.castleBlack.rookMoved<KING_SIDE>();
                else if (Utils::popLSBCopy(capture) == A8)
                    board.castleBlack.rookMoved<QUEEN_SIDE>();
            }

        } else if (board.queens.getBitboard<enemy>() & capture) {
            board.queens.flipSquare<enemy>(Utils::popLSBCopy(capture));
            board.status = board.material.registerPieceCapture<side, QUEEN_POINTS>(board.status);
        } else {
            puts("*PANIC, CAPTURE DOES NOT EXIST*");
            std::cout << move;
            puts("****************************");
        }
    }

  public:
    explicit Move(const uint16_t _move = 0) : move{_move} {};
    virtual void makeMove(Board &board) noexcept {
        board.halfmoveClock++;

        if (board.turn == BLACK) {
            board.fullmoveNumber++;
            board.turn = WHITE;
            board.pinnedMaskHVWhite = board.computePinMaskHV<WHITE>();
            board.pinnedMaskD12White = board.computePinMaskD12<WHITE>();
            board.checkMaskWhite = board.computeCheckMask<WHITE>();
            board.dangerTableWhite = board.computeDangerTable<WHITE>();

        } else {
            board.turn = BLACK;
            board.pinnedMaskHVBlack = board.computePinMaskHV<BLACK>();
            board.pinnedMaskD12Black = board.computePinMaskD12<BLACK>();
            board.checkMaskBlack = board.computeCheckMask<BLACK>();
            board.dangerTableBlack = board.computeDangerTable<BLACK>();
        }
    }

    void setToSquare(const Square &toSquare) { Move::move = Move::move | uint16_t(toSquare) << 0xA; }

    void setFromSquare(const Square &fromSquare) { Move::move = Move::move | fromSquare << 0x4; }

    Move getMoveCopy() const { return Move{move}; }

    void resetMove() { move = 0; }

    void setCapture() { Move::move = Move::move | 0x4; }

    void setDoublePawnPush() { Move::move = Move::move | 0x1; }

    void setKingSideCastle() { Move::move = Move::move | 0x2; }

    void setQueenSideCastle() { Move::move = Move::move | 0x3; }

    void setEnPassantCapture() { Move::move = Move::move | 0x5; }

    void setKnightPromotion() { Move::move |= 8; }

    void setBishopPromotion() { Move::move |= 9; }

    void setRookPromotion() { Move::move |= 10; }

    void setQueenPromotion() { Move::move |= 11; }

    void setKnightPromotionCapture() { Move::move |= 12; }

    void setBishopPromotionCapture() { Move::move |= 13; }

    void setRookPromotionCapture() { Move::move |= 14; }

    void setQueenPromotionCapture() { Move::move |= 15; }

    [[nodiscard]] uint8_t getTo() const { return move >> 0xA; }

    [[nodiscard]] uint8_t getFrom() const { return (move & 0x03F0) >> 0x4; }

    [[nodiscard]] uint8_t getFlags() const { return move & 0x000F; }

    [[nodiscard]] bool isPromotion() const { return (move & 8) == 8; }

    [[nodiscard]] bool isCapture() const { return (move & 0x4) == 0x4; }

    [[nodiscard]] bool isDoublePush() const { return (move & 0x1) == 0x1; }

    [[nodiscard]] bool isKingSideCastle() const { return (move & 0x2) == 0x2; }

    [[nodiscard]] bool isQueenSideCastle() const { return (move & 0x3) == 0x3; }

    [[nodiscard]] bool isKnightPromotion() const { return (Move::move & 8) == 8; }

    [[nodiscard]] bool isBishopPromotion() const { return (Move::move & 9) == 9; }

    [[nodiscard]] bool isRookPromotion() const { return (Move::move & 10) == 10; }

    [[nodiscard]] bool isKnightPromotionCapture() const { return (Move::move & 12) == 12; }

    [[nodiscard]] bool isBishopPromotionCapture() const { return (Move::move & 13) == 13; }

    [[nodiscard]] bool isRookPromotionCapture() const { return (Move::move & 14) == 14; }

    [[nodiscard]] bool isQueenPromotionCapture() const { return (Move::move & 15) == 15; }

    [[nodiscard]] bool isQueenPromotion() const { return (Move::move & 11) == 11; }

    [[nodiscard]] bool isEnPassant() const { return (Move::move & 0x5) == 5; }

    friend std::ostream &operator<<(std::ostream &os, const Move &_move) {

        os << Utils::squareToString(static_cast<Square>(_move.getFrom())) << Utils::squareToString(static_cast<Square>(_move.getTo()));
        if (_move.isPromotion()) {
            if (_move.isQueenPromotion() or _move.isQueenPromotionCapture()) {
                os << 'q';
            } else if (_move.isRookPromotionCapture() or _move.isRookPromotion()) {
                os << 'r';
            } else if (_move.isBishopPromotionCapture() or _move.isBishopPromotion()) {
                os << 'b';
            } else if (_move.isKnightPromotion() or _move.isKnightPromotionCapture()) {
                os << 'n';
            }
        }
        return os;
    }
    virtual ~Move() = default;
};

#endif // CHESS_ENGINE_MOVE_H
