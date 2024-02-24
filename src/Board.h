#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Board.h"
#include "Colors.h"
#include "FEN.h"
#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "MagicValuesParallelGenerator.h"
#include "Piece/JumpingPiece.h"
#include "Piece/Piece.h"
#include "Piece/SlidingPiece.h"
#include "Squares.h"
#include "Utils.h"

class Board {
  public:
    King king;
    Queen queens;
    Rook rooks;
    Bishop bishops;
    Knight knights;
    Pawn pawns;
    /**
     * true -> it's white's turn
     */
    bool turn;
    /**
     * last bit -> can white castle castle king side?
     * second bit -> can white castle queen side?
     * etc.
     */
    std::bitset<4> castle;
    uint64_t enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

    [[nodiscard]] uint64_t getFullWhiteSquares() const;

    [[nodiscard]] uint64_t getFullBlackSquares() const;

    [[nodiscard]] uint64_t getEmptySquares() const;

    void printBoard(std::ostream &os) const;

    void printStatus(std::ostream &os) const;

    template <Color side>
    [[nodiscard]] bool bishopAttacksSquare(const Square &square) const {
        uint64_t enemyBishops, enemyQueen;
        if constexpr (side == WHITE) {
            enemyBishops = bishops.getBlack();
            enemyQueen = queens.getBlack();
        } else {
            enemyBishops = bishops.getWhite();
            enemyQueen = queens.getWhite();
        }

        const uint64_t originBishopAttack = Bishop::getMoves(square, getEmptySquares());
        return (originBishopAttack & enemyBishops) | (originBishopAttack & enemyQueen);
    }

    template <Color side>
    [[nodiscard]] bool knightAttacksSquare(const Square &square) const {
        uint64_t enemyKing;
        if constexpr (side == WHITE) {
            enemyKing = king.getBlack();
        } else {
            enemyKing = king.getWhite();
        }

        const uint64_t originKingAttack = Knight::getMoves(square);
        return originKingAttack & enemyKing;
    }

    template <Color side>
    [[nodiscard]] bool kingAttacksSquare(const Square &square) const {
        uint64_t enemyKing;
        if constexpr (side == WHITE) {
            enemyKing = king.getBlack();
        } else {
            enemyKing = king.getWhite();
        }

        const uint64_t originKingAttack = King::getMoves(square);
        return originKingAttack & enemyKing;
    }

    template <Color side>
    [[nodiscard]] bool pawnAttacksSquare(const Square &square) const {
        uint64_t enemyPawns;
        if constexpr (side == WHITE) {
            enemyPawns = pawns.getBlack();
        } else {
            enemyPawns = pawns.getWhite();
        }

        const uint64_t originPawnAttack = Pawn::getMoves<side>(square);
        return originPawnAttack & enemyPawns;
    }

    template <Color side>
    [[nodiscard]] bool rookAttacksSquare(const Square &square) const {
        uint64_t enemyRooks, enemyQueen;
        if constexpr (side == WHITE) {
            enemyRooks = rooks.getBlack();
            enemyQueen = queens.getBlack();
        } else {
            enemyRooks = rooks.getWhite();
            enemyQueen = queens.getWhite();
        }

        const uint64_t originRookAttack = Rook::getMoves(square, getEmptySquares());
        return (originRookAttack & enemyRooks) | (originRookAttack & enemyQueen);
    }

  public:
    explicit Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')}, queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')}, bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')}, pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)}, castle{FEN::parseCastle(fen)}, enPassant{FEN::parseEnPassant(fen)},
          halfmoveClock{FEN::parseHalfmoveClock(fen)}, fullmoveNumber{FEN::parseFullmoveNumber(fen)} {};

    ~Board() = default;

    template <Color side>
    [[nodiscard]] bool isSquareAttacked(const Square &square) const {
        return rookAttacksSquare<side>(square) || bishopAttacksSquare<side>(square) || pawnAttacksSquare<side>(square) ||
               kingAttacksSquare<side>(square);
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#endif // CHESS_ENGINE_BOARD_H
