#ifndef CHESS_ENGINE_EVALUATION_H
#define CHESS_ENGINE_EVALUATION_H

#include "Enums/Colors.h"
#include "NNUE/nnue.h"

class Evaluation {
  public:
    Evaluation() {

        //        nnue_init("src/NNUE/nn-6b4236f2ec01.nnue");
        //        nnue_init("src/NNUE/nn-1ceb1ade0001.nnue");
        nnue_init("src/NNUE/nn-a269515ca2ff.nnue");
    };

    int evaluate(const Board &board) {
        const int perspective = (board.turn == WHITE) ? 1 : -1;
        const int eval = computeWhiteEvaluation(board) - computeBlackEvaluation(board);
        return eval * perspective + board.callNNUE();
    }

    int computeBlackEvaluation(const Board &board) {
        int eval = 0;
        eval += countMaterial<BLACK>(board);
        return eval;
    }
    int computeWhiteEvaluation(const Board &board) {
        int eval = 0;
        eval += countMaterial<WHITE>(board);
        return eval;
    }

    static constexpr uint16_t RookValue = 500;
    static constexpr uint16_t QueenValue = 900;
    static constexpr uint16_t BishopValue = 320;
    static constexpr uint16_t KnightValue = 300;
    static constexpr uint16_t PawnValue = 100;

    template <Color side>
    int countMaterial(const Board &board) {
        int material = 0;
        material += std::popcount(board.queens.getBitboard<side>()) * QueenValue;
        material += std::popcount(board.rooks.getBitboard<side>()) * RookValue;
        material += std::popcount(board.bishops.getBitboard<side>()) * BishopValue;
        material += std::popcount(board.knights.getBitboard<side>()) * KnightValue;
        material += std::popcount(board.pawns.getBitboard<side>()) * PawnValue;

        return material;
    }
};

#endif // CHESS_ENGINE_EVALUATION_H
