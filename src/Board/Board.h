#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include <cstdint>
#include <iostream>

#include "Enums/Colors.h"
#include "Enums/Squares.h"
#include "FEN.h"
#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "MagicValuesParallelGenerator.h"
#include "Move/MoveBuilder.h"
#include "Piece/JumpingPiece.h"
#include "Piece/Piece.h"
#include "Piece/SlidingPiece.h"
#include "Piece/SpecialPiece.h"
#include "Utils.h"

class Board {
    friend class LegalMove;

  public:
    template <Color side>
    [[nodiscard]] uint64_t getPinnedMaskHV() const;
    template <Color side>
    [[nodiscard]] uint64_t getPinnedMaskD12() const;
    template <Color side>
    [[nodiscard]] uint64_t getCheckMask() const;

    King king;
    Queen queens;
    Rook rooks;
    Bishop bishops;
    Knight knights;
    Pawn pawns;

    Color turn;

    Castle<WHITE> castleWhite;
    Castle<BLACK> castleBlack;

    uint64_t enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

  private:
    uint64_t pinnedMaskHVWhite;
    uint64_t pinnedMaskHVBlack;
    uint64_t pinnedMaskD12White;
    uint64_t pinnedMaskD12Black;
    uint64_t checkMaskWhite;
    uint64_t checkMaskBlack;
    uint64_t dangerTableWhite;
    uint64_t dangerTableBlack;

  public:
    template <Color side>
    [[nodiscard]] uint64_t getOccupiedSquares() const;

    [[nodiscard]] uint64_t getEmptySquares() const;

    void printBoard(std::ostream &os) const;

    void printStatus(std::ostream &os) const;

    [[nodiscard]] bool checkMoveIsEnPassant(const uint64_t &bitboard) { return enPassant & bitboard; }

    template <Color side>
    [[nodiscard]] uint64_t getCastleRightsBitboard() const noexcept;

    template <Color side>
    [[nodiscard]] bool isKingChecked() const noexcept {
        uint64_t kingBoard = king.getBitboard<side>();
        return isSquareAttacked<side>(Utils::popLSB(kingBoard));
    }

    template <Color side>
    [[nodiscard]] bool checkMoveIsPromotion(const uint64_t &bitboard) {
        if constexpr (side == WHITE) {
            return bitboard & Utils::LAST_ROW;
        } else {
            return bitboard & Utils::FIRST_ROW;
        }
    }

    template <Color side>
    [[nodiscard]] bool checkMoveIsCapture(const uint64_t &bitboard) {
        if constexpr (side == WHITE) {
            return bitboard & getOccupiedSquares<BLACK>();
        } else {
            return bitboard & getOccupiedSquares<WHITE>();
        }
    }

    template <Color side>
    [[nodiscard]] uint64_t knightAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t kingAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t pawnAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t rookAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t queenAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t isSquareAttacked(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t bishopAttacksSquare(const Square &) const;

    template <Color side>
    [[nodiscard]] uint64_t computePinMaskHV() const;

    template <Color side>
    [[nodiscard]] uint64_t computePinMaskD12() const;

    template <Color side>
    [[nodiscard]] uint64_t computeDangerTable() const;

    template <Color side>
    [[nodiscard]] uint64_t computeCheckMask() const {

        uint64_t kingBoard = king.getBitboard<side>();
        const Square kingSquare = Utils::popLSB(kingBoard);

        uint64_t checkMask = 0;
        if (uint64_t pawn = pawnAttacksSquare<side>(kingSquare)) {
            checkMask |= pawn;
        } else {
            checkMask |= knightAttacksSquare<side>(kingSquare);
        }
        uint8_t doubleAttacked = checkMask != 0;

        Utils::runForEachSetBit(bishopAttacksSquare<side>(kingSquare) | queenAttacksSquare<side>(kingSquare) |
                                    rookAttacksSquare<side>(kingSquare),
                                [&kingSquare, &checkMask, &doubleAttacked](const Square &square) -> void {
                                    checkMask |= Utils::getSetLineBetween(kingSquare, square);
                                    doubleAttacked++;
                                });
        // no checks
        if (!checkMask) {
            return -1;
        }
        return doubleAttacked == 2 ? 0 : checkMask; // 0 on double check
    };

  public:
    explicit Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')}, queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')}, bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')}, pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)}, castleWhite{FEN::parseCastle<WHITE>(fen)}, castleBlack{FEN::parseCastle<BLACK>(fen)},
          enPassant{FEN::parseEnPassant(fen)}, halfmoveClock{FEN::parseHalfmoveClock(fen)}, fullmoveNumber{FEN::parseFullmoveNumber(fen)},
          pinnedMaskHVWhite{computePinMaskHV<WHITE>()}, pinnedMaskHVBlack{computePinMaskHV<BLACK>()},
          pinnedMaskD12White{computePinMaskD12<WHITE>()}, pinnedMaskD12Black{computePinMaskD12<BLACK>()},
          checkMaskWhite{computeCheckMask<WHITE>()}, checkMaskBlack{computeCheckMask<BLACK>()},
          dangerTableWhite{computeDangerTable<WHITE>()}, dangerTableBlack{computeDangerTable<BLACK>()} {};

    ~Board() = default;

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#include "Board.inl"

#endif // CHESS_ENGINE_BOARD_H