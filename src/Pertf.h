#ifndef CHESS_ENGINE_PERTF_H
#define CHESS_ENGINE_PERTF_H

#include <string>

class Pertf {
  public:
    template <Color side>
    [[nodiscard]] static uint32_t countNodes(const Board &board, const uint8_t &depth) noexcept {
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
            std::cout << '\t' << *move << '\n';

            move->makeMove(newBoard);
            nodes += countNodes<Utils::flipColor(side)>(newBoard, depth - 1);
        });

        return nodes;
    }
    // e2a6:
    //    d5e6: 41 mie
    //    d5e6: 46 el

    template <Color side>
    static void startTest(const char *fen, const char *depth) noexcept {
        fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 10 10";
        depth = "2";
        Board board{fen};
        LegalMove legalMove{board};
        std::optional<std::vector<std::shared_ptr<Move>>> moves;
        if constexpr (side == WHITE) {
            moves = legalMove.getLegalMoves<WHITE>();
        } else {
            moves = legalMove.getLegalMoves<BLACK>();
        }
        if (moves->empty()) {
            puts("Asd");
            return;
        }
        uint64_t totalNodes = 0;
        std::for_each(moves->begin(), moves->end(), [&depth, board, &totalNodes](const std::shared_ptr<Move> &move) -> void {
            std::cout << *move;
            Board newBoard{board};
            move->makeMove(newBoard);
            const uint32_t moveNodes = countNodes<Utils::flipColor(side)>(newBoard, atoi(depth) - 1);
            totalNodes += moveNodes;
            std::cout << ": " << moveNodes << "\n";
        });
        std::cout << '\n' << totalNodes << "\n";
    }
};

#endif // CHESS_ENGINE_PERTF_H
