#ifndef CHESS_ENGINE_BOARD_INL
#define CHESS_ENGINE_BOARD_INL

#include "../Enums/Colors.h"
#include "../Enums/Sides.h"
#include "../Piece/JumpingPiece.h"
#include "../Piece/SlidingPiece.h"
#include "../Piece/SpecialPiece.h"
#include "Board.h"

template <Color side>
uint64_t Board::getPinnedMaskHV() const {
    return side == BLACK ? pinnedMaskHVBlack : pinnedMaskHVWhite;
}

template <Color side>
uint64_t Board::getPinnedMaskD12() const {
    return side == BLACK ? pinnedMaskD12Black : pinnedMaskD12White;
}

template <Color side>
uint64_t Board::getCheckMask() const {
    return side == BLACK ? checkMaskBlack : checkMaskWhite;
}

template <Color side>
uint64_t Board::computePinMaskHV() const {
    constexpr Color enemyColor = Utils::flipColor(side);

    uint64_t kingBoard = king.getBitboard<side>();
    const Square kingSquare = Utils::popLSB(kingBoard);
    const uint64_t kingAsRookRay = Rook::getMoves(kingSquare, getEmptySquares());

    uint64_t pinnedHV = 0;
    Utils::runForEachSetBit(rooks.getBitboard<enemyColor>() | queens.getBitboard<enemyColor>(),
                            [&pinnedHV, &kingAsRookRay, &kingSquare, this](const Square &square) {
                                const uint64_t rookAsRookRay = Rook::getMoves(square, getEmptySquares());

                                if (kingAsRookRay & rookAsRookRay & getOccupiedSquares<side>()) {
                                    pinnedHV |= Utils::getSetLineBetween(kingSquare, square);
                                }
                            });
    return pinnedHV;
}

template <Color side>
uint64_t Board::getCastleRightsBitboard() const noexcept {
    if (isKingChecked<side>()) {
        return 0;
    }

    uint64_t castleBoard = 0;
    uint64_t emptySquares = getEmptySquares();
    if constexpr (side == WHITE) {
        if (castleWhite.hasRookMoved<QUEEN_SIDE>()) {
            if (emptySquares & Utils::setSquare(B1) && emptySquares & Utils::setSquare(C1) && emptySquares & Utils::setSquare(D1)) {
                if (!isSquareAttacked<side>(C1) && !isSquareAttacked<side>(D1)) {
                    castleBoard |= Utils::setSquare(C1);
                }
            }
        }

        if (castleWhite.hasRookMoved<KING_SIDE>()) {
            if (emptySquares & Utils::setSquare(F1) && emptySquares & Utils::setSquare(G1)) {
                if (!isSquareAttacked<side>(F1) && !isSquareAttacked<side>(G1)) {
                    castleBoard |= Utils::setSquare(G1);
                }
            }
        }

    } else {
        if (castleBlack.hasRookMoved<QUEEN_SIDE>()) {
            if (!castleWhite.hasRookMoved<QUEEN_SIDE>()) {
                if (emptySquares & Utils::setSquare(B8) && emptySquares & Utils::setSquare(C8) && emptySquares & Utils::setSquare(D8)) {
                    if (!isSquareAttacked<side>(C8) && !isSquareAttacked<side>(D8)) {
                        castleBoard |= Utils::setSquare(C8);
                    }
                }
            }

            if (castleBlack.hasRookMoved<KING_SIDE>()) {
                if (emptySquares & Utils::setSquare(F8) && emptySquares & Utils::setSquare(G8)) {
                    if (!isSquareAttacked<side>(F8) && !isSquareAttacked<side>(G8)) {
                        castleBoard |= Utils::setSquare(G8);
                    }
                }
            }
        }
    }
    return castleBoard;
}

template <Color side>
uint64_t Board::isSquareAttacked(const Square &square) const {
    return rookAttacksSquare<side>(square) | bishopAttacksSquare<side>(square) | pawnAttacksSquare<side>(square) |
           kingAttacksSquare<side>(square) | queenAttacksSquare<side>(square) | knightAttacksSquare<side>(square);
}

template <Color side>
uint64_t Board::queenAttacksSquare(const Square &square) const {
    const uint64_t enemyQueen = queens.getBitboard<Utils::flipColor(side)>();
    const uint64_t originQueenAttack = Rook::getMoves(square, getEmptySquares()) | Bishop::getMoves(square, getEmptySquares());

    return originQueenAttack & enemyQueen;
}

template <Color side>
uint64_t Board::rookAttacksSquare(const Square &square) const {
    const uint64_t enemyRooks = rooks.getBitboard<Utils::flipColor(side)>();
    const uint64_t originRookAttack = Rook::getMoves(square, getEmptySquares());

    return originRookAttack & enemyRooks;
}

template <Color side>
uint64_t Board::knightAttacksSquare(const Square &square) const {
    const uint64_t enemyKnight = knights.getBitboard<Utils::flipColor(side)>();
    const uint64_t originKingAttack = Knight::getMoves(square);

    return originKingAttack & enemyKnight;
}

template <Color side>
uint64_t Board::bishopAttacksSquare(const Square &square) const {
    const uint64_t enemyBishops = bishops.getBitboard<Utils::flipColor(side)>();
    const uint64_t originBishopAttack = Bishop::getMoves(square, getEmptySquares());

    return originBishopAttack & enemyBishops;
}

template <Color side>
uint64_t Board::kingAttacksSquare(const Square &square) const {
    const uint64_t enemyKing = king.getBitboard<Utils::flipColor(side)>();
    const uint64_t originKingAttack = King::getMoves(square);

    return originKingAttack & enemyKing;
}
template <Color side>
uint64_t Board::pawnAttacksSquare(const Square &square) const {
    const uint64_t enemyPawns = pawns.getBitboard<Utils::flipColor(side)>();
    const uint64_t originPawnAttack = Pawn::getThreatens<side>(square);

    return originPawnAttack & enemyPawns;
}

template <Color side>
uint64_t Board::computeDangerTable() const {
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
                                    getOccupiedSquares<Utils::flipColor(side)>() | (getOccupiedSquares<side>() ^ king.getBitboard<side>()));

        } else if (Utils::setSquare(square) & king.getBitboard<Utils::flipColor(side)>()) {
            danger |= King::getMoves(square);

        } else if (Utils::setSquare(square) & pawns.getBitboard<Utils::flipColor(side)>()) {
            danger |= Pawn::getThreatens<Utils::flipColor(side)>(square);

        } else if (Utils::setSquare(square) & knights.getBitboard<Utils::flipColor(side)>()) {
            danger |= Knight::getMoves(square);
        }
    };

    Utils::runForEachSetBit(getOccupiedSquares<Utils::flipColor(side)>(), computeDangers);

    return danger | (getOccupiedSquares<side>() ^ king.getBitboard<side>());
}

template <Color side>
uint64_t Board::computePinMaskD12() const {
    constexpr Color enemyColor = Utils::flipColor(side);

    uint64_t kingBoard = king.getBitboard<side>();
    const Square kingSquare = Utils::popLSB(kingBoard);
    const uint64_t kingAsBishopRay = Bishop::getMoves(kingSquare, getEmptySquares());

    uint64_t pinnedHV = 0;
    Utils::runForEachSetBit(bishops.getBitboard<enemyColor>() | queens.getBitboard<enemyColor>(),
                            [&pinnedHV, &kingAsBishopRay, &kingSquare, this](const Square &square) {
                                const uint64_t bishopRay = Bishop::getMoves(square, getEmptySquares());

                                if (kingAsBishopRay & bishopRay & getOccupiedSquares<side>()) {
                                    pinnedHV |= Utils::getSetLineBetween(kingSquare, square);
                                }
                            });

    return pinnedHV;
}

template <Color side>
[[nodiscard]] uint64_t Board::computeCheckMask() const {

    uint64_t kingBoard = king.getBitboard<side>();
    const Square kingSquare = Utils::popLSB(kingBoard);

    uint64_t checkMask = 0;
    if (uint64_t pawn = pawnAttacksSquare<side>(kingSquare)) {
        checkMask |= pawn;
    } else {
        checkMask |= knightAttacksSquare<side>(kingSquare);
    }
    uint8_t doubleAttacked = checkMask != 0;

    Utils::runForEachSetBit(bishopAttacksSquare<side>(kingSquare) | queenAttacksSquare<side>(kingSquare) |
                                rookAttacksSquare<side>(kingSquare),
                            [&kingSquare, &checkMask, &doubleAttacked](const Square &square) -> void {
                                checkMask |= Utils::getSetLineBetween(kingSquare, square);
                                doubleAttacked++;
                            });
    // no checks
    if (!checkMask) {
        return -1;
    }
    return doubleAttacked == 2 ? 0 : checkMask; // 0 on double check
}

#endif // CHESS_ENGINE_BOARD_INL
