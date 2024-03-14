#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Colors.h"
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
    friend class BoardAttack;

  public:
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

    template <Color side>
    [[nodiscard]] uint64_t getOccupiedSquares() const;

    [[nodiscard]] uint64_t getEmptySquares() const;

    void printBoard(std::ostream &os) const;

    void printStatus(std::ostream &os) const;

    [[nodiscard]] bool checkMoveIsEnPassant(const Square &pieceSquare, const uint64_t &bitboard) { return enPassant & bitboard; }

    template <Color side>
    [[nodiscard]] uint64_t getCastleRightsBitboard() const {
        if (isKingChecked<side>()) {
            return 0;
        }
        if constexpr (side == WHITE) {
            if (!castleWhite.hasRookMoved<QUEEN_SIDE>()) {

            }

            if (!castleWhite.hasRookMoved<KING_SIDE>()) {
            }


        } else {

        }
    }

    template <Color side>
    [[nodiscard]] bool isKingChecked() const noexcept {
        return isSquareAttacked<side>(Utils::popLSB(king.getBitboard<side>()));
    }

    template <Color side>
    [[nodiscard]] bool checkMoveIsPromotion(const Square &pieceSquare, const uint64_t &bitboard) {
        if constexpr (side == WHITE) {
            return bitboard & Utils::LAST_ROW;
        } else {
            return bitboard & Utils::FIRST_ROW;
        }
    }

    template <Color side>
    [[nodiscard]] bool checkMoveIsCapture(const Square &pieceSquare, const uint64_t &bitboard) {
        if constexpr (side == WHITE) {
            return bitboard & getOccupiedSquares<BLACK>();
        } else {
            return bitboard & getOccupiedSquares<WHITE>();
        }
    }

    template <Color side>
    [[nodiscard]] bool checkMoveIsCastle(const Square &pieceSquare, const uint64_t &bitboard) {
        return enPassant & bitboard;
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

    //    template <Color side>
    //    [[nodiscard]] uint64_t getPinnedSquares() const {
    //        uint64_t rooksRays, bishopsRays;
    //        uint64_t kingAsRookRay, kingAsBishopRay;
    //        if constexpr (side == WHITE) {
    //            rooksRays = rooks.getBlockedAttackPattern(rooks.getBitboard<BLACK>(), ~getEmptySquares());
    //            kingAsRookRay = rooks.getBlockedAttackPattern(king.getBitboard<WHITE>(), ~getEmptySquares());
    //            bishopsRays = bishops.getBlockedAttackPattern(Square(bishops.getBitboard<BLACK>()), ~getEmptySquares());
    //            kingAsBishopRay = bishops.getBlockedAttackPattern(Square(king.getBitboard<BLACK>()), ~getEmptySquares());
    //        } else {
    //            rooksRays = rooks.getBlockedAttackPattern(rooks.getBitboard<WHITE>(), ~getEmptySquares());
    //            kingAsRookRay = rooks.getBlockedAttackPattern(king.getBitboard<BLACK>(), ~getEmptySquares());
    //            bishopsRays = bishops.getBlockedAttackPattern(Square(bishops.getBitboard<WHITE>()), ~getEmptySquares());
    //            kingAsBishopRay = bishops.getBlockedAttackPattern(Square(king.getBitboard<WHITE>()), ~getEmptySquares());
    //        }
    //        return (rooksRays & kingAsRookRay);
    //    }

    template <Color side>
    [[nodiscard]] uint64_t getDangerTable() const;

  public:
    explicit Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')}, queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')}, bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')}, pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)}, castleWhite{FEN::parseCastle<WHITE>(fen)}, castleBlack{FEN::parseCastle<BLACK>(fen)},
          enPassant{FEN::parseEnPassant(fen)}, halfmoveClock{FEN::parseHalfmoveClock(fen)},
          fullmoveNumber{FEN::parseFullmoveNumber(fen)} {};

    ~Board() = default;

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#endif // CHESS_ENGINE_BOARD_H
