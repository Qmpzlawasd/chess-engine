#ifndef CHESS_ENGINE_GAME_H
#define CHESS_ENGINE_GAME_H
#include "Game/Evaluation.h"
#include "Game/Time.h"
#include "Logger.h"
#include "NNUE/nnue.h"
#include "PositionHash/PositionHash.h"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>

#include <limits>

class Game {
    Evaluation evaluation;
    static constexpr uint8_t maximumDepth = 15;

    template <Color side>
    std::shared_ptr<Move> iterativeDeepening(Board &board) {
        float bestEval = std::numeric_limits<float>::min();

        std::shared_ptr<Move> bestMove = nullptr;
        for (int depth = 1; depth <= maximumDepth; ++depth) {
            std::optional<std::vector<std::shared_ptr<Move>>> legalMoves = LegalMove{board}.getLegalMoves<side>();
            for (const std::shared_ptr<Move> &move : legalMoves.value_or(std::vector<std::shared_ptr<Move>>{})) {
                Board newBoard{board};
                move->makeMove(newBoard);

                float currentMoveEval =
                    alphaBetaSearch<side>(newBoard, depth - 1, std::numeric_limits<float>::min(), std::numeric_limits<float>::max());

                if (currentMoveEval > bestEval) {
                    bestEval = currentMoveEval;
                    bestMove = move;
                }

                if (time.checkTimeIsUp()) {
                    logger.log(std::format("DepthSearched= {}\n", depth));
                    return bestMove;
                }
            }
        }
        return bestMove;
    }

    template <Color side>
    float alphaBetaSearch(Board &board, const uint8_t &depth, float alpha, float beta) {
        if (depth == 0 or board.isGameOver()) {
            return evaluation.evaluate<side>(board);
        }

        if constexpr (side == WHITE) {
            float maxEval = std::numeric_limits<float>::min();
            std::optional<std::vector<std::shared_ptr<Move>>> legalMoves = LegalMove{board}.getLegalMoves<side>();
            for (const std::shared_ptr<Move> &move : legalMoves.value_or(std::vector<std::shared_ptr<Move>>{})) {
                Board newBoard{board};
                move->makeMove(newBoard);

                float eval = alphaBetaSearch<BLACK>(newBoard, depth - 1, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);

                if (time.checkTimeIsUp()) {
                    return maxEval;
                }

                if (beta <= alpha)
                    break;
            }
            return maxEval;

        } else {
            float minEval = std::numeric_limits<float>::max();
            std::optional<std::vector<std::shared_ptr<Move>>> legalMoves = LegalMove{board}.getLegalMoves<side>();
            for (const std::shared_ptr<Move> &move : legalMoves.value_or(std::vector<std::shared_ptr<Move>>{})) {
                Board newBoard{board};
                move->makeMove(newBoard);

                float eval = alphaBetaSearch<WHITE>(newBoard, depth - 1, alpha, beta);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);

                if (time.checkTimeIsUp()) {
                    return minEval;
                }

                if (beta <= alpha)
                    break;
            }
            return minEval;
        }
    }

  public:
    Time time;

    Game() : evaluation(), time() {}

    std::shared_ptr<Move> start(Board &board) {
        time.start();
        if (board.turn == WHITE) {
            return iterativeDeepening<WHITE>(board);
        } else {
            return iterativeDeepening<BLACK>(board);
        }
    }
};

#endif // CHESS_ENGINE_GAME_H
