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
            //            std::cout << '\t' << *move << '\n';

            move->makeMove(newBoard);
            nodes += countNodes<Utils::flipColor(side)>(newBoard, depth - 1);
        });

        return nodes;
    }

    template <Color side>
    static void startTest(const char *fen, const char *depth) noexcept {

        Board board{fen};
        LegalMove legalMove{board};
        std::optional<std::vector<std::shared_ptr<Move>>> moves;
        if constexpr (side == WHITE) {
            moves = legalMove.getLegalMoves<WHITE>();
        } else {
            moves = legalMove.getLegalMoves<BLACK>();
        }
        if (moves->empty()) {
            puts("PANIC");
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
