#ifndef CHESS_ENGINE_MOVE_H
#define CHESS_ENGINE_MOVE_H
#include "Enums/Squares.h"
#include "Utils.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
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

  public:
    explicit Move(uint16_t _move = 0) : move{_move} {};

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

    friend std::ostream &operator<<(std::ostream &os, const Move &_move) {
        os << static_cast<int>(_move.getFrom()) << "->" << static_cast<int>(_move.getTo()) << " " << static_cast<int>(_move.getFlags())
           << '\n';
        return os;
    }
};

#endif // CHESS_ENGINE_MOVE_H
