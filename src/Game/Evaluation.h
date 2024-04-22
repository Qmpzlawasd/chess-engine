#ifndef CHESS_ENGINE_EVALUATION_H
#define CHESS_ENGINE_EVALUATION_H

#include "Enums/Colors.h"

class Evaluation {
    float eval(Board &board) { return 1; }

  public:
    template <Color side>
    float evaluate(Board &board) {
        if constexpr (side == WHITE) {
            return eval(board);
        } else {
            return eval(board);
        }
    }
};

#endif // CHESS_ENGINE_EVALUATION_H
