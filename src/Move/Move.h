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
    void makeCapture(Board &board, const uint64_t &capture) noexcept {
        if (board.queens.getBitboard<side>() & capture) {
            board.queens.flipSquare<side>(Utils::popLSBCopy(capture));
        } else if (board.rooks.getBitboard<side>() & capture) {
            board.rooks.flipSquare<side>(Utils::popLSBCopy(capture));
        } else if (board.pawns.getBitboard<side>() & capture) {
            board.pawns.flipSquare<side>(Utils::popLSBCopy(capture));
        } else if (board.knights.getBitboard<side>() & capture) {
            board.knights.flipSquare<side>(Utils::popLSBCopy(capture));
        } else if (board.king.getBitboard<side>() & capture) {
            board.king.flipSquare<side>(Utils::popLSBCopy(capture));
        } else if (board.bishops.getBitboard<side>() & capture) {
            board.bishops.flipSquare<side>(Utils::popLSBCopy(capture));
        } else {
            puts("*PANIC, MOVE DOES NOT EXIST*");
            std::cout << move;
            puts("****************************");
        }
    }

  public:
    explicit Move(uint16_t _move = 0) : move{_move} {};
    virtual void makeMove(Board &board) noexcept { // call la final
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

    void setPromotion() { Move::move = Move::move | 0x8; }

    void setCapture() { Move::move = Move::move | 0x4; }

    void setDoublePawnPush() { Move::move = Move::move | 0x1; }

    void setKingSideCastle() { Move::move = Move::move | 0x2; }

    void setQueenSideCastle() { Move::move = Move::move | 0x3; }

    void setEnPassantCapture() { Move::move = Move::move | 0x5; }

    void setEnPassant() { Move::move = Move::move | 0x5; }

    void setWithKnight() const { (void)0; }

    void setWithBishop() { Move::move = Move::move | 0x1; }

    void setWithRook() { Move::move = Move::move | 0x2; }

    void setWithQueen() { Move::move = Move::move | 0x3; }

    [[nodiscard]] uint8_t getTo() const { return move >> 0xA; }

    [[nodiscard]] uint8_t getFrom() const { return (move & 0x03F0) >> 0x4; }

    [[nodiscard]] uint8_t getFlags() const { return move & 0x000F; }

    [[nodiscard]] bool isCapture() const { return move & 0x4; }

    [[nodiscard]] bool isPromotion() const { return move & 0x8; }

    [[nodiscard]] bool isDoublePush() const { return move & 0x1; }

    [[nodiscard]] bool isKingSideCastle() const { return move & 0x2; }

    [[nodiscard]] bool isQueenSideCastle() const { return move & 0x3; }

    friend std::ostream &operator<<(std::ostream &os, const Move &_move) {

        os << Utils::squareToString(static_cast<Square>(_move.getFrom())) << "->"
           << Utils::squareToString(static_cast<Square>(_move.getTo())) << " " << static_cast<int>(_move.getFlags()) << '\n';
        return os;
    }
    virtual ~Move() = default;
};

#endif // CHESS_ENGINE_MOVE_H
