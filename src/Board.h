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
#include "Move/MoveBuilder.h"
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
    [[nodiscard]] uint64_t bishopAttacksSquare(const Square &square) const {
        uint64_t enemyBishops;
        if constexpr (side == WHITE) {
            enemyBishops = bishops.getBlack();
        } else {
            enemyBishops = bishops.getWhite();
        }

        const uint64_t originBishopAttack = Bishop::getMoves(square, getEmptySquares());
        return originBishopAttack & enemyBishops;
    }

     std::vector<Move> getMovesFromBitboard(const Square &pieceSquare, uint64_t bitboard) {
        MoveBuilder moveBuilder;
        std::vector<Move> generatedMoves;
        while (bitboard){
            const Square to =  Utils::popLSB(bitboard);
//            Utils::showBitBoard(Utils::setSquare(to));
            generatedMoves.push_back(moveBuilder.fromSquare(pieceSquare).toSquare(to).getMove());
        }
        return generatedMoves;
    }

    //    template <Color side>
    //    [[nodiscard]] uint64_t getPinnedAlliedPieces() const noexcept {
    //
    //
    //    }

    //    template <Color side>
    //    [[nodiscard]] uint64_t getPinnedSquares() const {
    //        uint64_t rooksRays, bishopsRays;
    //        uint64_t kingAsRookRay, kingAsBishopRay;
    //        if constexpr (side == WHITE) {
    //            rooksRays = rooks.getBlockedAttackPattern(rooks.getBlack(), ~getEmptySquares());
    //            kingAsRookRay = rooks.getBlockedAttackPattern(king.getWhite(), ~getEmptySquares());
    //            bishopsRays = bishops.getBlockedAttackPattern(Square(bishops.getBlack()), ~getEmptySquares());
    //            kingAsBishopRay = bishops.getBlockedAttackPattern(Square(king.getBlack()), ~getEmptySquares());
    //        } else {
    //            rooksRays = rooks.getBlockedAttackPattern(rooks.getWhite(), ~getEmptySquares());
    //            kingAsRookRay = rooks.getBlockedAttackPattern(king.getBlack(), ~getEmptySquares());
    //            bishopsRays = bishops.getBlockedAttackPattern(Square(bishops.getWhite()), ~getEmptySquares());
    //            kingAsBishopRay = bishops.getBlockedAttackPattern(Square(king.getWhite()), ~getEmptySquares());
    //        }
    //        return (rooksRays & kingAsRookRay);
    //    }
    template <Color side>
    [[nodiscard]] uint64_t knightAttacksSquare(const Square &square) const {
        uint64_t enemyKnight;
        if constexpr (side == WHITE) {
            enemyKnight = knights.getBlack();
        } else {
            enemyKnight = knights.getWhite();
        }

        const uint64_t originKingAttack = Knight::getMoves(square);
        return originKingAttack & enemyKnight;
    }

    template <Color side>
    [[nodiscard]] uint64_t kingAttacksSquare(const Square &square) const {
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
    [[nodiscard]] uint64_t pawnAttacksSquare(const Square &square) const {
        uint64_t enemyPawns;
        if constexpr (side == WHITE) {
            enemyPawns = pawns.getBlack();
        } else {
            enemyPawns = pawns.getWhite();
        }

        const uint64_t originPawnAttack = Pawn::getAttacks<side>(square);
        return originPawnAttack & enemyPawns;
    }

    template <Color side>
    [[nodiscard]] uint64_t rookAttacksSquare(const Square &square) const {
        uint64_t enemyRooks;
        if constexpr (side == WHITE) {
            enemyRooks = rooks.getBlack();
        } else {
            enemyRooks = rooks.getWhite();
        }

        const uint64_t originRookAttack = Rook::getMoves(square, getEmptySquares());
        return originRookAttack & enemyRooks;
    }
    template <Color side>
    [[nodiscard]] uint64_t queenAttacksSquare(const Square &square) const {
        uint64_t enemyQueen;
        if constexpr (side == WHITE) {
            enemyQueen = queens.getBlack();
        } else {
            enemyQueen = queens.getWhite();
        }

        const uint64_t originQueenAttack = Rook::getMoves(square, getEmptySquares()) | Bishop::getMoves(square, getEmptySquares());
        return originQueenAttack & enemyQueen;
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
    [[nodiscard]] uint64_t isSquareAttacked(const Square &square) const {
        return rookAttacksSquare<side>(square) | bishopAttacksSquare<side>(square) | pawnAttacksSquare<side>(square) |
               kingAttacksSquare<side>(square) | queenAttacksSquare<side>(square) | knightAttacksSquare<side>(square);
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#endif // CHESS_ENGINE_BOARD_H
