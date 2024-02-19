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
#include "Piece.h"
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

    template <Color side> [[nodiscard]] bool bishopAttacksSquare(const Square &square) const {
        uint64_t enemyBishops, enemyQueen;
        if constexpr (side == WHITE) {
            enemyBishops = bishops.getBlack();
            enemyQueen = queens.getBlack();
        } else {
            enemyBishops = bishops.getWhite();
            enemyQueen = queens.getWhite();
        }

        const uint64_t originBishopAttack = this->getBishopMoves<side>(square);
        return (originBishopAttack & enemyBishops) | (originBishopAttack & enemyQueen);
    }

    template <Color side> [[nodiscard]] bool knightAttacksSquare(const Square &square) const {
        uint64_t enemyKing;
        if constexpr (side == WHITE) {
            enemyKing = king.getBlack();
        } else {
            enemyKing = king.getWhite();
        }

        const uint64_t originKingAttack = this->getKingMoves<side>(square);
        return originKingAttack & enemyKing;
    }
    template <Color side> [[nodiscard]] bool kingAttacksSquare(const Square &square) const {
        uint64_t enemyKing;
        if constexpr (side == WHITE) {
            enemyKing = king.getBlack();
        } else {
            enemyKing = king.getWhite();
        }

        const uint64_t originKingAttack = this->getKingMoves<side>(square);
        return originKingAttack & enemyKing;
    }

    template <Color side> [[nodiscard]] bool pawnAttacksSquare(const Square &square) const {
        uint64_t enemyPawns;
        if constexpr (side == WHITE) {
            enemyPawns = pawns.getBlack();
        } else {
            enemyPawns = pawns.getWhite();
        }

        const uint64_t originPawnAttack = this->getPawnMoves<side>(square);
        return originPawnAttack & enemyPawns;
    }

    template <Color side> [[nodiscard]] bool rookAttacksSquare(const Square &square) const {
        uint64_t enemyRooks, enemyQueen;
        if constexpr (side == WHITE) {
            enemyRooks = rooks.getBlack();
            enemyQueen = queens.getBlack();
        } else {
            enemyRooks = rooks.getWhite();
            enemyQueen = queens.getWhite();
        }

        const uint64_t originRookAttack = this->getRookMoves<side>(square);
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

    template <Color side> [[nodiscard]] bool isSquareAttacked(const Square &square) const {
        return rookAttacksSquare<side>(square) || bishopAttacksSquare<side>(square) || pawnAttacksSquare<side>(square) ||
               kingAttacksSquare<side>(square);
    }

    template <Color side> [[nodiscard]] uint64_t getBishopMoves(const Square &square) const {
        const uint64_t blockerPattern = bishops.getNaiveAttackPattern(square) & ~getEmptySquares();
        Utils::showBitBoard(blockerPattern);
        uint64_t allyPieces;
        if constexpr (side == WHITE)
            allyPieces = this->getFullWhiteSquares();
        else
            allyPieces = this->getFullBlackSquares();

        const uint64_t optimisedIndex = blockerPattern * Bishop::MAGIC_CONSTANTS[square] >> Bishop::SHIFT_VALUE;
        return magicBitboard.bishopMoveTable[square][optimisedIndex] & ~allyPieces;
    }

    template <Color side> [[nodiscard]] uint64_t getKnightMoves(const Square &square) const {
        const uint64_t pieceBitboard = Utils::setSquare(square);

        uint64_t allyPieces;
        if constexpr (side == WHITE)
            allyPieces = this->getFullWhiteSquares();
        else
            allyPieces = this->getFullBlackSquares();

        constexpr uint64_t B_FILE = Utils::A_FILE << 1;
        constexpr uint64_t G_FILE = Utils::H_FILE >> 1;

        constexpr uint64_t SECOND_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * 1;
        constexpr uint64_t EIGHTH_ROW = Utils::FIRST_ROW << Utils::ROW_NUMBER * (Utils::ROW_NUMBER - 1);
        constexpr uint64_t SEVENTH_ROW = EIGHTH_ROW >> 1;

        const uint64_t attackLeftUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::A_FILE)) << (Utils::ROW_NUMBER * 2 - 1);
        const uint64_t attackLeftUpLeft = (pieceBitboard & ~(Utils::A_FILE | B_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER - 2);
        const uint64_t attackLeftDownDown =
            (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::A_FILE)) >> (Utils::ROW_NUMBER * 2 + 1);
        const uint64_t attackLeftDownLeft = (pieceBitboard & ~(Utils::A_FILE | Utils::FIRST_ROW | B_FILE)) >> (Utils::ROW_NUMBER + 2);

        const uint64_t attackRightUpUp = (pieceBitboard & ~(SEVENTH_ROW | EIGHTH_ROW | Utils::H_FILE)) << (Utils::ROW_NUMBER * 2 + 1);
        const uint64_t attackRightUpRight = (pieceBitboard & ~(Utils::H_FILE | G_FILE | EIGHTH_ROW)) << (Utils::ROW_NUMBER + 2);

        const uint64_t attackRightDownDown =
            (pieceBitboard & ~(Utils::FIRST_ROW | SECOND_ROW | Utils::H_FILE)) >> (Utils::ROW_NUMBER * 2 - 1);
        const uint64_t attackRightDownRight = (pieceBitboard & ~(Utils::H_FILE | Utils::FIRST_ROW | G_FILE)) >> (Utils::ROW_NUMBER - 2);

        return (attackLeftUpUp | attackLeftUpLeft | attackLeftDownDown | attackLeftDownLeft | attackRightUpUp | attackRightUpRight |
                attackRightDownDown | attackRightDownRight) &
               ~allyPieces;
    }

    template <Color side> [[nodiscard]] uint64_t getRookMoves(const Square &square) const {
        const uint64_t blockerPattern = rooks.getNaiveAttackPattern(square) & ~getEmptySquares();
        uint64_t allyPieces;
        if constexpr (side == WHITE)
            allyPieces = this->getFullWhiteSquares();
        else
            allyPieces = this->getFullBlackSquares();

        const uint64_t optimisedIndex = blockerPattern * Rook::MAGIC_CONSTANTS[square] >> Rook::SHIFT_VALUE;
        return magicBitboard.rookMoveTable[square][optimisedIndex] & ~allyPieces;
    }

    template <Color side> [[nodiscard]] uint64_t getPawnMoves(const Square &square) const {
        const uint64_t pieceBitboard = Utils::setSquare(square);

        uint64_t forwardMove, attackLeft, attackRight;
        if constexpr (side == WHITE) {
            forwardMove = pieceBitboard << Utils::ROW_NUMBER;
            attackLeft = ((pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1)) & (getFullBlackSquares() | enPassant);
            attackRight = ((pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1)) & (getFullBlackSquares() | enPassant);
        } else {
            // move orientations are reversed
            forwardMove = pieceBitboard >> Utils::ROW_NUMBER;
            attackLeft = ((pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1)) & (getFullWhiteSquares() | enPassant);
            attackRight = ((pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1)) & (getFullWhiteSquares() | enPassant);
        }
        forwardMove = forwardMove & getEmptySquares();

        return forwardMove | attackLeft | attackRight;
    }

    template <Color side> [[nodiscard]] uint64_t getKingMoves(const Square &square) const {
        const uint64_t pieceBitboard = Utils::setSquare(square);

        uint64_t allyPieces;
        if constexpr (side == WHITE)
            allyPieces = this->getFullWhiteSquares();
        else
            allyPieces = this->getFullBlackSquares();

        const uint64_t attackLeft = (pieceBitboard & ~Utils::A_FILE) >> 1;
        const uint64_t attackRight = (pieceBitboard & ~Utils::H_FILE) << 1;

        const uint64_t attackUp = (pieceBitboard >> Utils::ROW_NUMBER);
        const uint64_t attackDown = (pieceBitboard << Utils::ROW_NUMBER);

        const uint64_t attackLeftUp = (pieceBitboard & ~Utils::A_FILE) << (Utils::ROW_NUMBER - 1);
        const uint64_t attackLeftDown = (pieceBitboard & ~Utils::A_FILE) >> (Utils::ROW_NUMBER + 1);
        const uint64_t attackRightUp = (pieceBitboard & ~Utils::H_FILE) << (Utils::ROW_NUMBER + 1);
        const uint64_t attackRightDown = (pieceBitboard & ~Utils::H_FILE) >> (Utils::ROW_NUMBER - 1);

        return (attackLeftUp | attackLeftDown | attackRightUp | attackRightDown | attackLeft | attackRight | attackUp | attackDown) &
               ~allyPieces;
    }

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#endif // CHESS_ENGINE_BOARD_H
