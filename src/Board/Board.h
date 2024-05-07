#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>

#include "Enums/BoardStatus.h"
#include "Enums/Colors.h"
#include "Enums/Pieces.h"
#include "Enums/Squares.h"
#include "FEN/FEN.h"
#include "Logger.h"
#include "Material.h"
#include "NNUE/nnue.h"
#include "Piece/JumpingPiece.h"
#include "Piece/Piece.h"
#include "Piece/SlidingPiece.h"
#include "Piece/SpecialPiece.h"
#include "Utils.h"

class Board {
    friend class LegalMove;
    friend class Move;

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
    Material material;
    BoardStatus status;

    Castle<WHITE> castleWhite;
    Castle<BLACK> castleBlack;

    uint64_t enPassant;
    uint8_t halfmoveClock;
    uint16_t fullmoveNumber;

  protected:
    uint64_t hash;

    uint64_t pinnedMaskHVWhite;
    uint64_t pinnedMaskHVBlack;
    uint64_t pinnedMaskD12White;
    uint64_t pinnedMaskD12Black;
    uint64_t checkMaskWhite;
    uint64_t checkMaskBlack;
    uint64_t dangerTableWhite;
    uint64_t dangerTableBlack;

    //    std::queue<std::shared_ptr<Move>> moveHistory;

  public:
    //    [[nodiscard]] bool isThreefoldRepetition() const noexcept {
    //
    //        return ;
    //    };

    [[nodiscard]] bool isGameOver() const noexcept {
        return status != IN_PROGRESS and status != INSUFFICIENT_MATERIAL_WHITE and status != INSUFFICIENT_MATERIAL_BLACK;
    };

    [[nodiscard]] uint64_t getHash() const { return hash; };

    void setHash(const uint64_t &_hash) { Board::hash = _hash; };

    void resetEnPassant() noexcept { enPassant = 0; };

    void setEnPassant(const uint64_t &_enPassant) noexcept { enPassant = _enPassant; };

    template <Color side>
    [[nodiscard]] uint64_t getOccupiedSquares() const;

    [[nodiscard]] uint64_t getEmptySquares() const;

    void printBoard(std::ostream &os) const;

    void printStatus(std::ostream &os) const;

    void registerDraw() noexcept { status = DRAW; }

    void resetHalfMoveClock() noexcept { halfmoveClock = 0; }

    bool checkDraw50MoveRule() noexcept {
        if (halfmoveClock >= 100) {
            status = DRAW;
            return true;
        }
        return false;
    }

    template <Color side>
    void registerCheckmate() noexcept {
        if constexpr (side == WHITE) {
            status = WIN_WHITE;
        } else {
            status = WIN_BLACK;
        }
    }

    [[nodiscard]] int callNNUE() const noexcept {
        int pieces[33], squares[33], player;
        if (turn == WHITE) {
            player = 0;
        } else {
            player = 1;
        }
        uint8_t index = 2;

        for (int r = 7; r >= 0; r--) {
            for (int f = 0; f <= 7; f++) {
                int i = r * 8 + f;
                if (Utils::setSquare(static_cast<Square>(i)) & king.getBitboard<WHITE>()) {
                    pieces[0] = KING_WHITE + 1;
                    squares[0] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & king.getBitboard<BLACK>()) {
                    pieces[1] = KING_BLACK + 1;
                    squares[1] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & rooks.getBitboard<BLACK>()) {
                    pieces[index] = ROOK_BLACK + 1;
                    squares[index++] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & rooks.getBitboard<WHITE>()) {
                    pieces[index] = ROOK_WHITE + 1;
                    squares[index++] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & queens.getBitboard<WHITE>()) {
                    pieces[index] = QUEEN_WHITE + 1;
                    squares[index++] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & queens.getBitboard<BLACK>()) {
                    pieces[index] = QUEEN_BLACK + 1;
                    squares[index++] = i;

                } else if (Utils::setSquare(static_cast<Square>(i)) & bishops.getBitboard<WHITE>()) {
                    pieces[index] = BISHOP_WHITE + 1;

                    squares[index++] = i;
                } else if (Utils::setSquare(static_cast<Square>(i)) & bishops.getBitboard<BLACK>()) {
                    pieces[index] = BISHOP_BLACK + 1;

                    squares[index++] = i;
                } else if (Utils::setSquare(static_cast<Square>(i)) & pawns.getBitboard<WHITE>()) {
                    pieces[index] = PAWN_WHITE + 1;

                    squares[index++] = i;
                } else if (Utils::setSquare(static_cast<Square>(i)) & pawns.getBitboard<BLACK>()) {

                    pieces[index] = PAWN_BLACK + 1;
                    squares[index++] = i;
                } else if (Utils::setSquare(static_cast<Square>(i)) & knights.getBitboard<WHITE>()) {
                    pieces[index] = KNIGHT_WHITE + 1;

                    squares[index++] = i;
                } else if (Utils::setSquare(static_cast<Square>(i)) & knights.getBitboard<BLACK>()) {
                    pieces[index] = KNIGHT_BLACK + 1;

                    squares[index++] = i;
                }
            }
        }

        pieces[index] = 0;
        squares[index] = 0;
        return nnue_evaluate(player, pieces, squares);
    };

    template <Color side>
    [[nodiscard]] uint64_t getCastleRightsBitboard() const noexcept;

    template <Color side>
    [[nodiscard]] bool isKingChecked() const noexcept {
        uint64_t kingBoard = king.getBitboard<side>();
        return isSquareAttacked<side>(Utils::popLSB(kingBoard));
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
    [[nodiscard]] uint64_t computeCheckMask() const;

  public:
    explicit Board(const std::string &fen = "1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        : king{FEN::parsePiece(fen, 'K'), FEN::parsePiece(fen, 'k')}, queens{FEN::parsePiece(fen, 'Q'), FEN::parsePiece(fen, 'q')},
          rooks{FEN::parsePiece(fen, 'R'), FEN::parsePiece(fen, 'r')}, bishops{FEN::parsePiece(fen, 'B'), FEN::parsePiece(fen, 'b')},
          knights{FEN::parsePiece(fen, 'N'), FEN::parsePiece(fen, 'n')}, pawns{FEN::parsePiece(fen, 'P'), FEN::parsePiece(fen, 'p')},
          turn{FEN::parseTurn(fen)}, material{FEN::getMaterialValue<BLACK>(fen), FEN::getMaterialValue<WHITE>(fen)},
          status{material.init()}, castleWhite{FEN::parseCastle<WHITE>(fen)},
          castleBlack{FEN::parseCastle<BLACK>(fen)}, enPassant{FEN::parseEnPassant(fen)}, halfmoveClock{FEN::parseHalfmoveClock(fen)},
          fullmoveNumber{FEN::parseFullmoveNumber(fen)}, hash{0}, pinnedMaskHVWhite{computePinMaskHV<WHITE>()},
          pinnedMaskHVBlack{computePinMaskHV<BLACK>()}, pinnedMaskD12White{computePinMaskD12<WHITE>()},
          pinnedMaskD12Black{computePinMaskD12<BLACK>()}, checkMaskWhite{computeCheckMask<WHITE>()},
          checkMaskBlack{computeCheckMask<BLACK>()}, dangerTableWhite{computeDangerTable<WHITE>()}, dangerTableBlack{
                                                                                                        computeDangerTable<BLACK>()} {};

    ~Board() = default;

    friend std::ostream &operator<<(std::ostream &os, const Board &board) {
        board.printStatus(os);
        board.printBoard(os);
        return os;
    }
};

#include "Board.inl"

#endif // CHESS_ENGINE_BOARD_H
