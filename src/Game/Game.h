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
    int bestMoveEvaluation;

    uint64_t numNodes;
    uint64_t numCutoffs;

    static constexpr uint8_t maximumDepth = 15;

    template <Color side>
    void iterativeDeepening(Board &board) {
        this->bestMove = nullptr;
        this->numNodes = 0;
        this->numCutoffs = 0;
        //        this->bestMoveEvaluation = 0;
        int depth;
        for (depth = 1; depth <= maximumDepth; ++depth) {
            alphaBetaSearch<side>(board, depth, 0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

            if (time.checkTimeIsUp()) {
                break;
            }
        }
        std::stringstream ss;
        ss << "DepthSearched= " << depth << " Evaluation= " << bestMoveEvaluation << " NodesSearched= " << numNodes
           << " Cutoffs= " << numCutoffs << '\n';
        logger.log(ss.str());
    }

    template <Color side>
    int alphaBetaSearch(Board &board, const uint8_t &depth, const uint64_t &ply, int alpha, int beta) {

        if (depth == 0 or board.isGameOver()) {
            return evaluation.evaluate(board);
        }
        std::optional<std::vector<std::shared_ptr<Move>>> legalMoves;
        if constexpr (side == WHITE) {
            legalMoves = LegalMove{board}.getLegalMoves<WHITE>();
        } else {
            legalMoves = LegalMove{board}.getLegalMoves<BLACK>();
        }

        auto moves = legalMoves.value_or(std::vector<std::shared_ptr<Move>>{});
        //                std::ranges::sort(moves, [&board](const std::shared_ptr<Move> &a, const std::shared_ptr<Move> &b) -> bool {
        //
        //                    if (a->isCapture() or b->isCapture()) {
        //                        return a->isCapture() and not b->isCapture();
        //                    }
        //                    uint64_t enemyPieces;
        //                    if constexpr (side == WHITE) {
        //                        enemyPieces = board.getOccupiedSquares<BLACK>();
        //                    } else {
        //                        enemyPieces = board.getOccupiedSquares<WHITE>();
        //                    }
        //                    return (enemyPieces & Utils::setSquare(static_cast<Square>(a->getTo()))) and not  (enemyPieces &
        //                    Utils::setSquare(static_cast<Square>(b->getTo()))) ;
        //
        //                });

        int bestScore = std::numeric_limits<int>::min();
        for (const std::shared_ptr<Move> &move : moves) {
            if (time.checkTimeIsUp()) {
                break;
            }
            Board newBoard{board};
            move->makeMove(newBoard);
            int score = -alphaBetaSearch<Utils::flipColor(side)>(newBoard, depth - 1, ply + 1, -beta, -alpha);
            if (ply == 0) {
                logger.log(move->toString(), " -> ");
                logger.log(score);
            }
            numNodes++;
            if (score > bestScore) {
                alpha = score;
                if (ply == 0) {
                    bestMove = move;
                    bestMoveEvaluation = score;
                }
                bestScore = score;
            }

            if (bestScore > alpha) {
                alpha = bestScore;
            }

            if (alpha >= beta) {
                numCutoffs++;
                return beta;
            }
        }

        return bestScore;
    }

  public:
    Time time;

    Game() : evaluation{}, bestMove{nullptr}, bestMoveEvaluation{0}, numNodes{0}, numCutoffs{0}, time{} {}

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
