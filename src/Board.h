//
// Created by stefan on 25/01/24.
//

#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include "Board.h"
#include "FEN.h"
#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include "MagicValuesParallelGenerator.h"
#include "Piece.h"
#include "Squares.h"
#include "Utils.h"
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>

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
     * last bit -> can white castle  castle kingside
     * second bit -> can white castle queenside
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

  public:
    explicit Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')}, queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')}, bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')}, pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)}, castle{FEN::parseCastle(fen)}, enPassant{FEN::parseEnPassant(fen)},
          halfmoveClock{FEN::parseHalfmoveClock(fen)}, fullmoveNumber{FEN::parseFullmoveNumber(fen)} {};

    ~Board() = default;

    template <bool side> [[nodiscard]] uint64_t getKightMoves(const Square &) const;

    [[nodiscard]] uint64_t getRookMoves(const Square &square) const;

    [[nodiscard]] uint64_t getBishopMoves(const Square &square) const;

    template <bool side> [[nodiscard]] uint64_t getPawnMoves(const Square &) const;

    template <bool side> [[nodiscard]] uint64_t getKingMoves(const Square &) const;

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#endif // CHESS_ENGINE_BOARD_H
