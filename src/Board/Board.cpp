#include "Board.h"

template <Color side>
[[nodiscard]] uint64_t Board::getOccupiedSquares() const {
    return king.getBitboard<side>() | queens.getBitboard<side>() | rooks.getBitboard<side>() | bishops.getBitboard<side>() |
           knights.getBitboard<side>() | pawns.getBitboard<side>();
}

[[nodiscard]] uint64_t Board::getEmptySquares() const { return ~(getOccupiedSquares<WHITE>() | getOccupiedSquares<BLACK>()); }

void Board::printBoard(std::ostream &os) const {}

void Board::printStatus(std::ostream &os) const {
    //    os << "Move " << fullmoveNumber << ": " << (castle[1] ? "Q" : "") << (castle[3] ? "q" : "") << "\t" << (int)halfmoveClock << "\t"
    //       << enPassant << "\t" << (castle[0] ? "K" : "") << (castle[2] ? "k" : "") << "\n";
}

template <Color side>
[[nodiscard]] uint64_t Board::getDangerTable() const {
    uint64_t danger = 0;

    auto computeDangers = [&danger, this](const Square &square) -> void {
        if (Utils::setSquare(square) & rooks.getBitboard<Utils::flipColor(side)>()) {
            danger |=
                Rook::getThreatens(square,
                                   getOccupiedSquares<Utils::flipColor(side)>() | (getOccupiedSquares<side>() ^ king.getBitboard<side>()));

                    } else if (Utils::setSquare(square) & bishops.getBitboard<Utils::flipColor(side)>()) {
                        danger |= Bishop::getThreatens(square,
                                                       getOccupiedSquares<Utils::flipColor(side)>() |
                                                           (getOccupiedSquares<side>() ^ king.getBitboard<side>()));

                    } else if (Utils::setSquare(square) & queens.getBitboard<Utils::flipColor(side)>()) {
                        danger |=
                            Queen::getThreatens(square,
                                                getOccupiedSquares<Utils::flipColor(side)>() | (getOccupiedSquares<side>() ^
                                                king.getBitboard<side>()));

                    } else if (Utils::setSquare(square) & king.getBitboard<Utils::flipColor(side)>()) {
                        danger |= King::getMoves(square);

                    } else if (Utils::setSquare(square) & pawns.getBitboard<Utils::flipColor(side)>()) {
                        danger |= Pawn::getThreatens<Utils::flipColor(side)>(square);

                    } else if (Utils::setSquare(square) & knights.getBitboard<Utils::flipColor(side)>()) {
                        danger |= Knight::getMoves(square);
        }
    };

    Utils::runForEachSetBit(getOccupiedSquares<Utils::flipColor(side)>(), computeDangers);
    return danger |(getOccupiedSquares<side>() ^ king.getBitboard<side>());
}

template uint64_t Board::getDangerTable<WHITE>() const;
template uint64_t Board::getDangerTable<BLACK>() const;

#include "Board.h"
#include "../Colors.h"
#include "../Piece/JumpingPiece.h"
#include "../Piece/SlidingPiece.h"
#include "../Piece/SpecialPiece.h"
#include "../Utils.h"
#include <cstdint>

template <Color side>
[[nodiscard]] uint64_t Board::isSquareAttacked(const Square &square) const {
    return rookAttacksSquare<side>(square) | bishopAttacksSquare<side>(square) | pawnAttacksSquare<side>(square) |
           kingAttacksSquare<side>(square) | queenAttacksSquare<side>(square) | knightAttacksSquare<side>(square);
}

template uint64_t Board::isSquareAttacked<WHITE>(const Square &) const;
template uint64_t Board::isSquareAttacked<BLACK>(const Square &) const;

template <Color side>
[[nodiscard]] uint64_t Board::queenAttacksSquare(const Square &square) const {
    const uint64_t enemyQueen = queens.getBitboard<Utils::flipColor(side)>();
    const uint64_t originQueenAttack = Rook::getMoves(square, getEmptySquares()) | Bishop::getMoves(square, getEmptySquares());

    return originQueenAttack & enemyQueen;
}

template uint64_t Board::queenAttacksSquare<WHITE>(const Square &) const;
template uint64_t Board::queenAttacksSquare<BLACK>(const Square &) const;

template <Color side>
[[nodiscard]] uint64_t Board::rookAttacksSquare(const Square &square) const {
    const uint64_t enemyRooks = rooks.getBitboard<Utils::flipColor(side)>();
    const uint64_t originRookAttack = Rook::getMoves(square, getEmptySquares());

    return originRookAttack & enemyRooks;
}

template uint64_t Board::rookAttacksSquare<WHITE>(const Square &square) const;
template uint64_t Board::rookAttacksSquare<BLACK>(const Square &square) const;

template <Color side>
[[nodiscard]] uint64_t Board::knightAttacksSquare(const Square &square) const {
    const uint64_t enemyKnight = knights.getBitboard<Utils::flipColor(side)>();
    const uint64_t originKingAttack = Knight::getMoves(square);

    return originKingAttack & enemyKnight;
}

template uint64_t Board::knightAttacksSquare<WHITE>(const Square &square) const;
template uint64_t Board::knightAttacksSquare<BLACK>(const Square &square) const;

template <Color side>
[[nodiscard]] uint64_t Board::kingAttacksSquare(const Square &square) const {
    const uint64_t enemyKing = king.getBitboard<Utils::flipColor(side)>();
    const uint64_t originKingAttack = King::getMoves(square);

    return originKingAttack & enemyKing;
}

template uint64_t Board::kingAttacksSquare<WHITE>(const Square &square) const;
template uint64_t Board::kingAttacksSquare<BLACK>(const Square &square) const;

template <Color side>
[[nodiscard]] uint64_t Board::pawnAttacksSquare(const Square &square) const {
    const uint64_t enemyPawns = pawns.getBitboard<Utils::flipColor(side)>();
    const uint64_t originPawnAttack = Pawn::getThreatens<side>(square);

    return originPawnAttack & enemyPawns;
}

template uint64_t Board::pawnAttacksSquare<WHITE>(const Square &square) const;
template uint64_t Board::pawnAttacksSquare<BLACK>(const Square &square) const;

template <Color side>
[[nodiscard]] uint64_t Board::bishopAttacksSquare(const Square &square) const {
    const uint64_t enemyBishops = bishops.getBitboard<Utils::flipColor(side)>();
    const uint64_t originBishopAttack = Bishop::getMoves(square, getEmptySquares());

    return originBishopAttack & enemyBishops;
}

template uint64_t Board::bishopAttacksSquare<WHITE>(const Square &square) const;
template uint64_t Board::bishopAttacksSquare<BLACK>(const Square &square) const;
