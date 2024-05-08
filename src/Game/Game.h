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
#include <sstream>
#include <sys/stat.h>

class Game {
    Evaluation evaluation;
    std::shared_ptr<Move> bestMove;

    static constexpr uint8_t maximumDepth = 100;
    uint8_t currentDepth = 0;
    int ply = 0;
    template <Color side>
    void iterativeDeepening(Board &board) {
        this->bestMove = nullptr;
        this->ply = 0;
        for (int depth = 1; depth <= maximumDepth; ++depth) {
            this->currentDepth = depth;
            logger.log(ply);
            alphaBetaSearch<side>(board, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

            if (time.checkTimeIsUp()) {
                std::stringstream ss;
                ss << "DepthSearched= " << depth << '\n';
                logger.log(ss.str());
                return;
            }
        }
    }

    template <Color side>
    int alphaBetaSearch(Board &board, const uint8_t &depth, int alpha, int beta) {
        if (depth == 0 or board.isGameOver()) {
            return evaluation.evaluate(board);
        }

        std::shared_ptr<Move> bestMoveSoFar = nullptr;

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
            ply++;
            int score = -alphaBetaSearch<side>(newBoard, depth - 1, -alpha, -beta);

            if (score >= beta) {
                return beta;
            }

            if (score > alpha) {
                alpha = score;
                if (depth == currentDepth) {
                    bestMove = move;
                }
            }

            //            if (time.checkTimeIsUp()) {
            //                return alpha;
            //            }
        }

        return alpha;
    }

  public:
    Time time;

    Game() : evaluation(), bestMove(nullptr), currentDepth(0), time() {}

    std::shared_ptr<Move> start(Board &board) {
        time.start();
        if (board.turn == WHITE) {
            iterativeDeepening<WHITE>(board);
        } else {
            iterativeDeepening<BLACK>(board);
        }
        return bestMove;
    }
};

#endif // CHESS_ENGINE_GAME_H
