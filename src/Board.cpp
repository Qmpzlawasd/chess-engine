#pragma once
#include "Utils.cpp"
#include "Piece.cpp"

class Board
{
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
     * last bit -> can white castle kingside
     * second bit -> can white castle queenside
     * etc.
     */
    std::bitset<4> castle;
    uint8_t enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

    const __uint128_t getFullBoard() const
    {
        return ((__uint128_t)(king.getBlack()) |
                (queens.getBlack()) |
                (knights.getBlack()) |
                (bishops.getBlack()) |
                (pawns.getBlack()) |
                (rooks.getBlack()))
                   << (Utils::COLUMN_NUMBER * Utils::ROW_NUMBER) |
               (king.getWhite()) |
               (queens.getWhite()) |
               (knights.getWhite()) |
               (bishops.getWhite()) |
               (pawns.getWhite()) |
               (rooks.getWhite());
    }

public:
    Board(const std::string &fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq f1 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')},
          queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')},
          bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')},
          pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::getTurn(fen)},
          castle{FEN::getCastle(fen)},
          enPassant{FEN::getEnPassant(fen)},
          halfmoveClock{FEN::getHalfmoveClock(fen)},
          fullmoveNumber{FEN::getFullmoveNumber(fen)} {};

    ~Board() = default;
    void makeMove() {};
};
