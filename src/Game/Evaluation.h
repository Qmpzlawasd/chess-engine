#ifndef CHESS_ENGINE_EVALUATION_H
#define CHESS_ENGINE_EVALUATION_H

#include "Enums/Colors.h"
#include "NNUE/nnue.h"
#include <algorithm>
#include <iostream>

class Evaluation {
    static float endGameWeight;
    static constexpr uint8_t queenEndgameWeight = 45;
    static constexpr uint8_t rookEndgameWeight = 20;
    static constexpr uint8_t bishopEndgameWeight = 10;
    static constexpr uint8_t knightEndgameWeight = 10;
    static constexpr uint8_t ENDGAME_START_WEIGHT =
        2 * rookEndgameWeight + 2 * bishopEndgameWeight + 2 * knightEndgameWeight + queenEndgameWeight;

    template <Color side>
    void setEndGameWeight(const Board &board) {
        float material = 0;
        material += std::popcount(board.queens.getBitboard<side>()) * queenEndgameWeight;
        material += std::popcount(board.rooks.getBitboard<side>()) * rookEndgameWeight;
        material += std::popcount(board.bishops.getBitboard<side>()) * bishopEndgameWeight;
        material += std::popcount(board.knights.getBitboard<side>()) * knightEndgameWeight;
        Evaluation::endGameWeight = 1 - std::min(1.f, material / static_cast<float>(ENDGAME_START_WEIGHT));
    }

    int computeBlackEvaluation(const Board &board) {
        setEndGameWeight<WHITE>(board);

        int eval = 0;
        eval += countMaterial<BLACK>(board);
        eval += countPiecesWeights<BLACK>(board);
        return eval;
    }

    int computeWhiteEvaluation(const Board &board) {
        setEndGameWeight<BLACK>(board);

        int eval = 0;
        eval += countMaterial<WHITE>(board);
        eval += countPiecesWeights<WHITE>(board);
        return eval;
    }

    static constexpr uint16_t RookValue = 500;
    static constexpr uint16_t QueenValue = 900;
    static constexpr uint16_t BishopValue = 320;
    static constexpr uint16_t KnightValue = 300;
    static constexpr uint16_t PawnValue = 100;

    template <Color side>
    int countMaterial(const Board &board, const bool &withWeights = true) {
        int material = 0;
        material += std::popcount(board.queens.getBitboard<side>()) * (withWeights ? QueenValue : 1);
        material += std::popcount(board.rooks.getBitboard<side>()) * (withWeights ? RookValue : 1);
        material += std::popcount(board.bishops.getBitboard<side>()) * (withWeights ? BishopValue : 1);
        material += std::popcount(board.knights.getBitboard<side>()) * (withWeights ? KnightValue : 1);
        material += std::popcount(board.pawns.getBitboard<side>()) * (withWeights ? PawnValue : 1);

        return material;
    }

    static constexpr std::array<int, 64> PAWN_EARLY_WEIGHTS = {
        0, 0, 0, 0,  0,  0, 0, 0, 50, 50, 50,  50, 50, 50,  50, 50, 10, 10, 20, 30,  30,  20, 10, 10, 5, 5, 10, 25, 25, 10, 5, 5,
        0, 0, 0, 20, 20, 0, 0, 0, 5,  -5, -10, 0,  0,  -10, -5, 5,  5,  10, 10, -20, -20, 10, 10, 5,  0, 0, 0,  0,  0,  0,  0, 0};

    static constexpr std::array<int, 64> PAWN_ENDGAME_WEIGHTS = {
        0,  0,  0,  0,  0,  0,  0,  0,  80, 80, 80, 80, 80, 80, 80, 80, 50, 50, 50, 50, 50, 50, 50, 50, 30, 30, 30, 30, 30, 30, 30, 30,
        20, 20, 20, 20, 20, 20, 20, 20, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 0,  0,  0,  0,  0,  0,  0,  0};

    static constexpr std::array<int, 64> ROOKS_WEIGHTS = {0, 0,  0,  0,  0,  0, 0, 0, 5, 10, 10, 10, 10, 10, 10, 5, -5, 0,  0,  0, 0, 0,
                                                          0, -5, -5, 0,  0,  0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0, 0,  -5, -5, 0, 0, 0,
                                                          0, 0,  0,  -5, -5, 0, 0, 0, 0, 0,  0,  -5, 0,  0,  0,  5, 5,  0,  0,  0};

    static constexpr std::array<int, 64> KNIGHT_WEIGHTS = {
        -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,   0,   -20, -40, -30, 0,   10,  15,  15, 10,
        0,   -30, -30, 5,   15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,   -30, -30, 5,   10, 15,
        15,  10,  5,   -30, -40, -20, 0,   5,   5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50,
    };

    static constexpr std::array<int, 64> BISHOP_WEIGHTS = {
        -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,   0,   0,   -10, -10, 0,   5,   10,  10, 5,
        0,   -10, -10, 5,   5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,   -10, -10, 10,  10, 10,
        10,  10,  10,  -10, -10, 5,   0,   0,   0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20,
    };

    static constexpr std::array<int, 64> QUEEN_WEIGHTS = {
        -20, -10, -10, -5, -5, -10, -10, -20, -10, 0, 0, 0, 0, 0, 0, -10, -10, 0, 5, 5, 5, 5, 0, -10, -5,  0,   5,   5,  5,  5,   0,   -5,
        0,   0,   5,   5,  5,  5,   0,   -5,  -10, 5, 5, 5, 5, 5, 0, -10, -10, 0, 5, 0, 0, 0, 0, -10, -20, -10, -10, -5, -5, -10, -10, -20};

    static constexpr std::array<int, 64> KING_EARLY_WEIGHTS = {
        -80, -70, -70, -70, -70, -70, -70, -80, -60, -60, -60, -60, -60, -60, -60, -60, -40, -50, -50, -60, -60, -50,
        -50, -40, -30, -40, -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30, -20, -10, -20, -20, -20,
        -20, -20, -20, -10, 20,  20,  -5,  -5,  -5,  -5,  20,  20,  20,  30,  10,  0,   0,   10,  30,  20};

    static constexpr std::array<int, 64> KING_ENDGAME_WEIGHTS = {
        -20, -10, -10, -10, -10, -10, -10, -20, -5,  0,   5,   5,   5,   5,   0,   -5,  -10, -5,  20,  30,  30, 20,
        -5,  -10, -15, -10, 35,  45,  45,  35,  -10, -15, -20, -15, 30,  40,  40,  30,  -15, -20, -25, -20, 20, 25,
        25,  20,  -20, -25, -30, -25, 0,   0,   0,   0,   -25, -30, -50, -30, -30, -30, -30, -30, -30, -50};

    template <Color side>
    int countPiecesWeights(const Board &board) {
        int totalWeights = 0;

        Utils::runForEachSetBit(board.queens.getBitboard<side>(), [&totalWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }

            totalWeights += QUEEN_WEIGHTS[line * 8 + column];
        });

        Utils::runForEachSetBit(board.rooks.getBitboard<side>(), [&totalWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            totalWeights += ROOKS_WEIGHTS[line * 8 + column];
        });

        Utils::runForEachSetBit(board.bishops.getBitboard<side>(), [&totalWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            totalWeights += BISHOP_WEIGHTS[line * 8 + column];
        });

        Utils::runForEachSetBit(board.knights.getBitboard<side>(), [&totalWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            totalWeights += KNIGHT_WEIGHTS[line * 8 + column];
        });

        int pawnWeights = 0;
        Utils::runForEachSetBit(board.pawns.getBitboard<side>(), [&pawnWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            pawnWeights += PAWN_EARLY_WEIGHTS[line * 8 + column];
        });

        totalWeights += pawnWeights * (1 - endGameWeight);

        int kingWeights = 0;
        Utils::runForEachSetBit(board.king.getBitboard<side>(), [&kingWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            kingWeights += KING_EARLY_WEIGHTS[line * 8 + column];
        });
        totalWeights += kingWeights * (1 - endGameWeight);

        pawnWeights = 0;
        Utils::runForEachSetBit(board.pawns.getBitboard<side>(), [&pawnWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            pawnWeights += PAWN_ENDGAME_WEIGHTS[line * 8 + column];
        });
        totalWeights += pawnWeights * endGameWeight;

        kingWeights = 0;
        Utils::runForEachSetBit(board.king.getBitboard<side>(), [&kingWeights](const Square &square) {
            auto &&[column, line] = Utils::getCoordinates(square);
            if constexpr (side == WHITE) {
                line = 7 - line;
            }
            kingWeights += KING_ENDGAME_WEIGHTS[line * 8 + column];
        });
        totalWeights += kingWeights * endGameWeight;

        return totalWeights;
    }

  public:
    Evaluation() { nnue_init("src/NNUE/nn-6b4236f2ec01.nnue"); };

    int evaluate(const Board &board) {
        const int eval = computeWhiteEvaluation(board) - computeBlackEvaluation(board);
        int perspective = board.turn == WHITE ? 1 : -1;
        return eval * perspective + board.callNNUE();
    }
};

float Evaluation::endGameWeight = 0;

#endif // CHESS_ENGINE_EVALUATION_H
