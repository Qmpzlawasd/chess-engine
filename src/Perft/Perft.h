#ifndef CHESS_ENGINE_PERFT_H
#define CHESS_ENGINE_PERFT_H

#include "Board/LegalMove.h"
#include "Game/Time.h"
#include "Move/Move.h"
#include "PositionHash/PositionHash.h"
#include <cassert>
#include <memory>
#include <string>

class Perft {
    static Time time;

    template <Color side>
    [[nodiscard]] static uint32_t countNodes(Board &board, const uint8_t &depth) noexcept {
        if (depth == 0)
            return 1;

        LegalMove legalMove{board};
        std::optional<std::vector<std::shared_ptr<Move>>> moves;

        if constexpr (side == WHITE) {
            moves = legalMove.getLegalMoves<WHITE>();
        } else {
            moves = legalMove.getLegalMoves<BLACK>();
        }

        uint32_t nodes = 0;
        std::for_each(moves->begin(), moves->end(), [&depth, board, &nodes](const std::shared_ptr<Move> &move) -> void {
            Board newBoard{board};
            move->makeMove(newBoard);
            nodes += countNodes<Utils::flipColor(side)>(newBoard, depth - 1);
        });

        return nodes;
    }

    static uint64_t startTest(const char *fen, const uint8_t &depth) noexcept {
        Board board{fen};
        LegalMove legalMove{board};
        std::optional<std::vector<std::shared_ptr<Move>>> moves;
        if (board.turn == WHITE) {
            moves = legalMove.getLegalMoves<WHITE>();
        } else {
            moves = legalMove.getLegalMoves<BLACK>();
        }
        if (moves->empty()) {
            return 0;
        }

        uint64_t totalNodes;
        time.start();
        if (board.turn == WHITE) {
            totalNodes = countNodes<WHITE>(board, depth);
        } else {
            totalNodes = countNodes<BLACK>(board, depth);
        }

        std::cout << "nodes: " << totalNodes << " nps: " << totalNodes / (time.stop() + 1) * 1000 << std::endl;
        return totalNodes;
    }

  public:
    static void runTests() {
        // en passant out of check
        assert(Perft::startTest("8/8/8/8/8/8/kp5Q/N6K b - - 0 1", 1) == 3);

        // starter position
        assert(Perft::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2) == 400);
        assert(Perft::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3) == 8902);
        assert(Perft::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4) == 197281);
        assert(Perft::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5) == 4865609);

        // https://www.chessprogramming.org/Perft_Results
        assert(Perft::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1) == 48);
        assert(Perft::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2) == 2039);
        assert(Perft::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3) == 97862);
        assert(Perft::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4) == 4085603);
        assert(Perft::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1) == 14);
        assert(Perft::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2) == 191);
        assert(Perft::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3) == 2812);
        assert(Perft::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4) == 43238);
        assert(Perft::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5) == 674624);

        // https://www.chessprogramming.net/perfect-perft/
        assert(Perft::startTest("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6) == 1134888);
        assert(Perft::startTest("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6) == 1440467);
        assert(Perft::startTest("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4) == 1274206);
        assert(Perft::startTest("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4) == 1720476);
        assert(Perft::startTest("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4) == 23527);

        // http://www.rocechess.ch/perft.html
        assert(Perft::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 1) == 24);
        assert(Perft::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 2) == 496);
        assert(Perft::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 3) == 9483);
        assert(Perft::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 4) == 182838);
        assert(Perft::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 5) == 3605103);
    }
};

#endif // CHESS_ENGINE_PERFT_H
