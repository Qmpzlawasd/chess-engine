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
        int depth;
        for (depth = 1; depth <= maximumDepth; ++depth) {
            alphaBetaSearch<side>(board, depth, 0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

            if (time.checkTimeIsUp()) {
                break;
            }
        }
        std::stringstream ss;
        ss << "DepthSearched= " << depth << " Evaluation= " << -bestMoveEvaluation << " NodesSearched= " << numNodes
           << " Cutoffs= " << numCutoffs << '\n';
        logger.log(ss.str());
    }

    template <Color side>
    int alphaBetaSearch(Board &board, const uint8_t &depth, const uint64_t &ply, int alpha, int beta) {
        if (depth == 0 or board.isGameOver()) {
            return evaluation.evaluate(board);
            return QuiescenceSearch(board, alpha, beta);
        }
        std::optional<std::vector<std::shared_ptr<Move>>> legalMoves;
        if (board.turn == WHITE) {
            legalMoves = LegalMove{board}.getLegalMoves<WHITE>();
        } else {
            legalMoves = LegalMove{board}.getLegalMoves<BLACK>();
        }

        auto moves = legalMoves.value_or(std::vector<std::shared_ptr<Move>>{});
        //        std::sort(moves.begin(), moves.end());
        for (const std::shared_ptr<Move> &move : moves) {
            Board newBoard{board};
            move->makeMove(newBoard);
            int score = -alphaBetaSearch<side>(newBoard, depth - 1, ply + 1, -beta, -alpha);
            numNodes++;
            if (score >= beta) {
                numCutoffs++;
                return beta;
            }

            if (score > alpha) {
                alpha = score;
                if (ply == 0) {
                    bestMove = move;
                    bestMoveEvaluation = score;
                }
            }
            if (time.checkTimeIsUp()) {
                return alpha;
            }
        }

        return alpha;
    }

    int QuiescenceSearch(Board &board, int alpha, int beta) {
        int eval = evaluation.evaluate(board);
        numNodes++;
        if (eval >= beta) {
            numCutoffs++;
            return beta;
        }
        if (eval > alpha) {
            alpha = eval;
        }

        std::optional<std::vector<std::shared_ptr<Move>>> legalMoves;
        if (board.turn == WHITE) {
            legalMoves = LegalMove{board}.getLegalMoves<WHITE>();
        } else {
            legalMoves = LegalMove{board}.getLegalMoves<BLACK>();
        }
        auto moves = legalMoves.value_or(std::vector<std::shared_ptr<Move>>{});
        logger.log(moves.size(), " -> ");
        moves.erase(std::remove_if(begin(moves),
                                   end(moves),
                                   [&board](const std::shared_ptr<Move> &move) {
                                       return Utils::setSquare(static_cast<Square>(move->getTo())) &
                                              (board.turn == WHITE ? board.getOccupiedSquares<BLACK>() : board.getOccupiedSquares<WHITE>());
                                   }),
                    end(moves));
        logger.log(moves.size());
        for (const std::shared_ptr<Move> &move : moves) {
            Board newBoard{board};
            move->makeMove(newBoard);
            eval = -QuiescenceSearch(newBoard, -beta, -alpha);

            if (eval >= beta) {
                numCutoffs++;
                return beta;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            if (time.checkTimeIsUp()) {
                return alpha;
            }
        }

        return alpha;
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
