#ifndef CHESS_ENGINE_GAME_H
#define CHESS_ENGINE_GAME_H

#include "PositionHash.h"
#include <limits>

class Game {

    template <Color side>
    static std::shared_ptr<Move> iterativeDeepening(Board &board, const uint8_t &maximumDepth) {
        std::shared_ptr<Move> bestMove = nullptr;
        float bestEval = std::numeric_limits<float>::min();
        for (int depth = 1; depth <= maximumDepth; ++depth) {
            for (const std::shared_ptr<Move> &move : LegalMove{board}.getLegalMoves<side>()) {
                Board newBoard{board};
                move->makeMove(board);
                float currentMoveEval = alphaBetaSearch<side>(board,
                                                              depth - 1,
                                                              std::numeric_limits<float>::min(),
                                                              std::numeric_limits<float>::max(),
                                                              Utils::flipColor(side));

                if (currentMoveEval > bestEval) {
                    bestEval = currentMoveEval;
                    bestMove = move;
                }
            }
            return bestMove;
        }
    }

    template <Color side>
    static float alphaBetaSearch(Board &board, const uint8_t &depth, float alpha, float beta) {
        if (depth == 0 or board.isGameOver()) {
            return evaluateBoard(board);
        }
        if constexpr (side == WHITE) {
            float maxEval = std::numeric_limits<float>::min();
            for (const std::shared_ptr<Move> &move : LegalMove{board}.getLegalMoves<side>()) {
                Board newBoard{board};
                move->makeMove(board);
                float eval = alphaBetaSearch<BLACK>(newBoard, depth - 1, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha)
                    break;
            }
            return maxEval;
        } else {
            float minEval = std::numeric_limits<float>::max();
            for (const std::shared_ptr<Move> &move : LegalMove{board}.getLegalMoves<side>()) {
                Board newBoard{board};
                move->makeMove(board);
                float eval = alphaBetaSearch<WHITE>(newBoard, depth - 1, alpha, beta);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha)
                    break;
            }
            return minEval;
        }
    }

    static float evaluateBoard(const Board &board) { return 1; }

  public:
    static void start() {
        const std::string fenString = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        Board board{fenString};
        PositionHash as{};
        auto &&[hash, isThere] = as.exists(board);
        std::cout << hash << ' ' << isThere << '\n';
        as.addHash(hash, 1);
        auto &&[hash2, isThere2] = as.exists(board);
        std::cout << hash2 << ' ' << isThere2 << '\n';
    }
};

#endif // CHESS_ENGINE_GAME_H
