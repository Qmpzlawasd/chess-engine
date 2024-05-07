#ifndef CHESS_ENGINE_GAME_H
#define CHESS_ENGINE_GAME_H

#include "Game/Evaluation.h"
#include "Game/Time.h"
#include "Logger.h"
#include "PositionHash/PositionHash.h"
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <limits>
#include <sys/stat.h>

class Game {
    Evaluation evaluation;
    static constexpr uint8_t maximumDepth = 15;

    std::shared_ptr<Move> iterativeDeepening(Board &board) {
        int bestEval;
        if (board.turn == WHITE) {
            bestEval = std::numeric_limits<int>::min();
        } else {
            bestEval = std::numeric_limits<int>::max();
        }

        std::shared_ptr<Move> bestMove = nullptr;
        for (int depth = 1; depth <= maximumDepth; ++depth) {

            std::optional<std::vector<std::shared_ptr<Move>>> legalMoves;
            if (board.turn == WHITE) {
                legalMoves = LegalMove{board}.getLegalMoves<WHITE>();
            } else {
                legalMoves = LegalMove{board}.getLegalMoves<BLACK>();
            }

            for (const std::shared_ptr<Move> &move : legalMoves.value_or(std::vector<std::shared_ptr<Move>>{})) {
                Board newBoard{(const Board)board};
                move->makeMove(newBoard);

                int currentMoveEval =
                    alphaBetaSearch(newBoard, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                if (board.turn == WHITE) {
                    if (currentMoveEval > bestEval) {
                        bestEval = currentMoveEval;
                        bestMove = move;
                    }
                } else {
                    if (currentMoveEval < bestEval) {
                        bestEval = currentMoveEval;
                        bestMove = move;
                    }
                }

                if (time.checkTimeIsUp()) {
                    logger.log(std::format("DepthSearched= {}, Evaluation= {}\n", depth, bestEval));
                    return bestMove;
                }
            }
        }
        return bestMove;
    }

    int alphaBetaSearch(Board &board, const uint8_t &depth, int alpha, int beta) {
        if (depth == 0 or board.isGameOver()) {
            return evaluation.evaluate(board);
        }

        int maxEval = std::numeric_limits<int>::min();

        std::optional<std::vector<std::shared_ptr<Move>>> legalMoves;
        if (board.turn == WHITE) {
            legalMoves = LegalMove{board}.getLegalMoves<WHITE>();
        } else {
            legalMoves = LegalMove{board}.getLegalMoves<BLACK>();
        }

        auto moves = legalMoves.value_or(std::vector<std::shared_ptr<Move>>{});
        //            std::sort(moves.begin(), moves.end());
        for (const std::shared_ptr<Move> &move : moves) {
            Board newBoard{board};

            move->makeMove(newBoard);
            int eval = -alphaBetaSearch(newBoard, depth - 1, -alpha, -beta);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, maxEval);

            if (time.checkTimeIsUp()) {
                return maxEval;
            }

            if (beta <= alpha)
                return alpha;
        }
        return maxEval;
    }

  public:
    Time time;

    Game() : evaluation(), time() {}

    std::shared_ptr<Move> start(Board &board) {
        time.start();
        return iterativeDeepening(board);
    }
};

#endif // CHESS_ENGINE_GAME_H
