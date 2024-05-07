#ifndef CHESS_ENGINE_EVALUATION_H
#define CHESS_ENGINE_EVALUATION_H

#include "Enums/Colors.h"
#include "NNUE/nnue.h"

class Evaluation {
  public:
    Evaluation() { nnue_init("src/NNUE/nn-6b4236f2ec01.nnue"); };

    int evaluate(const Board &board) { return board.callNNUE(); }
};

#endif // CHESS_ENGINE_EVALUATION_H
