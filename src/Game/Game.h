#ifndef CHESS_ENGINE_GAME_H
#define CHESS_ENGINE_GAME_H

#include "Game/Evaluation.h"
#include "Game/Time.h"
#include "PositionHash/PositionHash.h"
#include <limits>

class Game {
    static Evaluation evaluation;

    static Time time;

    static constexpr uint8_t maximumDepth = 3;

    template <Color side>
    static std::shared_ptr<Move> iterativeDeepening(Board &board) {
        std::shared_ptr<Move> bestMove = nullptr;
        float bestEval = std::numeric_limits<float>::min();

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
                    return bestMove;
                }
            }
        }
        return bestMove;
    }

    template <Color side>
    static float alphaBetaSearch(Board &board, const uint8_t &depth, float alpha, float beta) {
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
    static void start(std::string_view white, std::string_view black) {
        Board board{};
        std::shared_ptr<Move> bestMove;
        while (true) {
            time.start();
            if (board.turn == WHITE) {
                bestMove = iterativeDeepening<WHITE>(board);
            } else {
                bestMove = iterativeDeepening<BLACK>(board);
            }

            if (bestMove == nullptr) {
                break;
            }
            std::cout << *bestMove << ' ';

            bestMove->makeMove(board);
        }
        printResults(white, black, board);
    }

    static void printResults(std::string_view white, std::string_view black, const Board &board) {
        std::cout << white << " VS " << black << " verdict: " << static_cast<uint16_t>(board.status)
                  << " time: " << Time::allowedMilliseconds << '\n';
    }
};

Evaluation Game::evaluation;
Time Game::time;

#endif // CHESS_ENGINE_GAME_H
