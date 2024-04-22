#ifndef CHESS_ENGINE_PERTF_H
#define CHESS_ENGINE_PERTF_H

#include <cassert>
#include <memory>
#include <string>

class Pertf {
    PositionHash zobristMap;

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
            //            std::cout << *move << ' ' << board.status << '\n';
            move->makeMove(newBoard);
            //            std::cout << *move << ' ' << newBoard.status << "\n\n";
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
        //        PositionHash zobristMap;
        uint64_t totalNodes = 0;
        std::for_each(moves->begin(), moves->end(), [&depth, board, &totalNodes](const std::shared_ptr<Move> &move) -> void {
            Board newBoard{board};
            move->makeMove(newBoard);

            uint32_t moveNodes;
            if (board.turn == WHITE) {
                moveNodes = countNodes<BLACK>(newBoard, depth - 1);
            } else {
                moveNodes = countNodes<WHITE>(newBoard, depth - 1);
            }

            totalNodes += moveNodes;
            //            std::cout << *move << ": " << moveNodes << "\n";
        });
        //        std::cout << '\n' << totalNodes << "\n";
        return totalNodes;
    }

  public:
    static void runTests() {
        // en passant out of check
        assert(Pertf::startTest("8/8/8/8/8/8/kp5Q/N6K b - - 0 1", 7) == 1);

        // starter position
        assert(Pertf::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2) == 400);
        assert(Pertf::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3) == 8902);
        assert(Pertf::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4) == 197281);
        assert(Pertf::startTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5) == 4865609);

        // https://www.chessprogramming.org/Perft_Results
        assert(Pertf::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1) == 48);
        assert(Pertf::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2) == 2039);
        assert(Pertf::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3) == 97862);
        assert(Pertf::startTest("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4) == 4085603);
        assert(Pertf::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1) == 14);
        assert(Pertf::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2) == 191);
        assert(Pertf::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3) == 2812);
        assert(Pertf::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4) == 43238);
        assert(Pertf::startTest("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5) == 674624);

        // https://www.chessprogramming.net/perfect-perft/
        assert(Pertf::startTest("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6) == 1134888);
        assert(Pertf::startTest("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6) == 1440467);
        assert(Pertf::startTest("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4) == 1274206);
        assert(Pertf::startTest("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", 4) == 1720476);
        assert(Pertf::startTest("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4) == 23527);

        // http://www.rocechess.ch/perft.html
        assert(Pertf::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 1) == 24);
        assert(Pertf::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 2) == 496);
        assert(Pertf::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 3) == 9483);
        assert(Pertf::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 4) == 182838);
        assert(Pertf::startTest("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", 5) == 3605103);
    }
};

#endif // CHESS_ENGINE_PERTF_H
