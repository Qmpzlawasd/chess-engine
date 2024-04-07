#ifndef CHESS_ENGINE_LEGALMOVE_INL
#define CHESS_ENGINE_LEGALMOVE_INL

#include "../Move/MoveBuilder.h"
#include "LegalMove.h"
#include "Move/Move.h"

template <Color side>
[[nodiscard]] std::vector<Move> LegalMove::getLegalMoves() noexcept {
    std::vector<Move> allMoves;
    std::vector<Move> rook = getRookLegalMoves<side>();
    std::vector<Move> queen = getQueenLegalMoves<side>();
    std::vector<Move> knight = getKnightLegalMoves<side>();
    std::vector<Move> bishop = getBishopLegalMoves<side>();
    std::vector<Move> king = getKingLegalMoves<side>();
    std::vector<Move> pawn = getPawnLegalMoves<side>();

    allMoves.reserve(rook.size() + queen.size() + bishop.size() + king.size() + pawn.size() + knight.size());
    allMoves.insert(allMoves.end(), rook.begin(), rook.end());
    allMoves.insert(allMoves.end(), queen.begin(), queen.end());
    allMoves.insert(allMoves.end(), bishop.begin(), bishop.end());
    allMoves.insert(allMoves.end(), knight.begin(), knight.end());
    allMoves.insert(allMoves.end(), king.begin(), king.end());
    allMoves.insert(allMoves.end(), pawn.begin(), pawn.end());

    return allMoves;
}

template <Color side>
std::vector<Move> LegalMove::getKingLegalMoves() const noexcept {
    std::vector<Move> moves;

    uint64_t kingBoard = board.king.getBitboard<side>();
    const auto kingSquare = Utils::popLSB(kingBoard);

    MoveBuilder moveBuilder;

    const uint64_t basicMoves = ~board.computeDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());
    Utils::runForEachSetBit(basicMoves, [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
        moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
    });

    Utils::runForEachSetBit(board.getCastleRightsBitboard<side>(),
                            [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
                                if constexpr (side == WHITE) {
                                    if (square == Utils::KING_SIDE_CASTLE_WHITE) {
                                        moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withKingSideCastle());
                                        return;
                                    }
                                    if (square == Utils::QUEEN_SIDE_CASTLE_WHITE) {
                                        moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withQueenSideCastle());
                                        return;
                                    }
                                } else {
                                    if (square == Utils::QUEEN_SIDE_CASTLE_BLACK) {
                                        moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withQueenSideCastle());
                                        return;
                                    }
                                    if (square == Utils::KING_SIDE_CASTLE_BLACK) {
                                        moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).withKingSideCastle());
                                        return;
                                    }
                                }

                                moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
                            });

    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getRookLegalMoves() const noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;
    const uint64_t pinnedRooksCannotMove = board.rooks.getBitboard<side>() & board.getPinnedMaskD12<side>();

    const uint64_t pinnedRooks = board.rooks.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t freeRooks = board.rooks.getBitboard<side>() ^ (pinnedRooksCannotMove | pinnedRooks);

    Utils::runForEachSetBit(pinnedRooks, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Rook::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskHV<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withRook().getMove());
        });
    });
    Utils::runForEachSetBit(freeRooks, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Rook::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withRook().getMove());
        });
    });
    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMoves() noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;

    const uint64_t pawnsCannotAttack = board.pawns.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t pawnsCannotBePushed = board.pawns.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freePawns = board.pawns.getBitboard<side>() ^ (pawnsCannotBePushed | pawnsCannotAttack);

    Utils::runForEachSetBit(pawnsCannotAttack, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves =
            Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>() & board.getPinnedMaskHV<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove());
        });
    });

    Utils::runForEachSetBit(pawnsCannotBePushed, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | board.enPassant) &
                                                 board.getCheckMask<side>() & board.getPinnedMaskD12<side>();
        Utils::runForEachSetBit(legalMovesWithEnpassant, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withCapture().getMove());
        });
    });

    Utils::runForEachSetBit(freePawns, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t canDoEnPassant = handlePawnShenanigans<side>(originSquare) ? 0 : board.enPassant;

        const uint64_t legalMovesWithEnpassant = Pawn::getThreatens<side>(originSquare) &
                                                 (board.getOccupiedSquares<Utils::flipColor(side)>() | canDoEnPassant) &
                                                 board.getCheckMask<side>();
        const uint64_t legalPushes = Pawn::getMoves<side>(originSquare, board.getEmptySquares()) & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMovesWithEnpassant, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withCapture().withEnPassantCapture());
        });

        Utils::runForEachSetBit(legalPushes, [&originSquare, &moveBuilder, &moves](const Square &square) {
            if constexpr (side == WHITE) {

                if (Utils::setSquare(square) & Utils::DOUBLE_PUSH_RANK_WHITE && Utils::setSquare(square) & Utils::PAWNS_STARTER_WHITE) {
                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withDoublePawnPush());
                    return;
                }
            } else {
                if (Utils::setSquare(square) & Utils::DOUBLE_PUSH_RANK_BLACK && Utils::setSquare(square) & Utils::PAWNS_STARTER_BLACK) {
                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withDoublePawnPush());
                    return;
                }
            }

            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).getMove()); // not covered
        });
    });
    return moves;
}

template <Color side>
[[nodiscard]] bool LegalMove::handlePawnShenanigans(const Square &square) noexcept {
    if constexpr (side == WHITE) {
        if ((Utils::setSquare(square) & Utils::CHECK_ENPASSANT_WHITE) == 0) {
            return true;
        }
    } else {
        if ((Utils::setSquare(square) & Utils::CHECK_ENPASSANT_BLACK) == 0) {
            return true;
        }
    }
    uint64_t controversialPawn = 0;
    const Square kingSquare = Utils::popLSBCopy(board.king.getBitboard<side>());
    if constexpr (side == WHITE) {

        if (Utils::CHECK_ENPASSANT_WHITE & board.king.getBitboard<side>() &&
            Utils::CHECK_ENPASSANT_WHITE & (board.queens.getBitboard<BLACK>() | board.rooks.getBitboard<BLACK>())) {

            // we delete the two pawns and look for a check
            controversialPawn = board.enPassant >> Utils::COLUMN_NUMBER;
            board.pawns.flipSquare<BLACK>(Utils::popLSBCopy(controversialPawn));
            board.pawns.flipSquare<WHITE>(square);

            const uint64_t underAttack = board.queenAttacksSquare<side>(kingSquare) | board.rookAttacksSquare<side>(kingSquare);

            // undo
            board.pawns.flipSquare<WHITE>(square);
            board.pawns.flipSquare<BLACK>(Utils::popLSB(controversialPawn));

            return underAttack;
        }
    } else {
        if (Utils::CHECK_ENPASSANT_BLACK & board.king.getBitboard<side>() &&
            Utils::CHECK_ENPASSANT_BLACK & (board.queens.getBitboard<WHITE>() | board.rooks.getBitboard<WHITE>())) {
            controversialPawn = board.enPassant << Utils::ROW_NUMBER;
            board.pawns.flipSquare<WHITE>(Utils::popLSBCopy(controversialPawn));
            board.pawns.flipSquare<BLACK>(square);

            const uint64_t underAttack = board.queenAttacksSquare<side>(kingSquare) | board.rookAttacksSquare<side>(kingSquare);

            board.pawns.flipSquare<BLACK>(square);
            board.pawns.flipSquare<WHITE>(Utils::popLSB(controversialPawn));

            return underAttack;
        }
    }

    if constexpr (side == WHITE) {
        controversialPawn = board.enPassant >> Utils::COLUMN_NUMBER;
        board.pawns.flipSquare<BLACK>(Utils::popLSBCopy(controversialPawn));
    } else {
        controversialPawn = board.enPassant << Utils::COLUMN_NUMBER;
        board.pawns.flipSquare<WHITE>(Utils::popLSBCopy(controversialPawn));
    }

    const uint64_t enemyQueen = board.queens.getBitboard<Utils::flipColor(side)>();
    const uint64_t originQueenAttack = Bishop::getMoves(kingSquare, board.getEmptySquares());
    const uint64_t underAttack = board.bishopAttacksSquare<side>(kingSquare) | (enemyQueen & originQueenAttack);

    // undo
    if constexpr (side == WHITE) {
        board.pawns.flipSquare<BLACK>(Utils::popLSB(controversialPawn));
    } else {
        board.pawns.flipSquare<WHITE>(Utils::popLSB(controversialPawn));
    }

    return underAttack;
}

template <Color side>
std::vector<Move> LegalMove::getQueenLegalMoves() const noexcept {
    std::vector<Move> moves;
    MoveBuilder moveBuilder;

    const uint64_t pinnedQueensHV = board.queens.getBitboard<side>() & board.getPinnedMaskHV<side>();
    const uint64_t pinnedQueensD12 = board.queens.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freeQueens = board.queens.getBitboard<side>() ^ (pinnedQueensD12 | pinnedQueensHV);

    Utils::runForEachSetBit(pinnedQueensD12, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskD12<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(pinnedQueensHV, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskHV<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(freeQueens, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Queen::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getKnightLegalMoves() const noexcept {
    std::vector<Move> moves;
    const uint64_t freeKnights = board.knights.getBitboard<side>() ^
                                 (board.knights.getBitboard<side>() & (board.getPinnedMaskD12<side>() | board.getPinnedMaskHV<side>()));
    MoveBuilder moveBuilder = MoveBuilder{};

    Utils::runForEachSetBit(freeKnights, [&moveBuilder, this, &moves](const Square &originSquare) -> void {
        Utils::runForEachSetBit(Knight::getMoves(originSquare) & ~board.getOccupiedSquares<side>() & board.getCheckMask<side>(),
                                [&moveBuilder, &originSquare, &moves](const Square &legalSquare) -> void {
                                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(legalSquare).withKnight().getMove());
                                });
    });
    return moves;
}

template <Color side>
std::vector<Move> LegalMove::getBishopLegalMoves() const noexcept {

    std::vector<Move> moves;
    MoveBuilder moveBuilder;
    const uint64_t pinnedBishopsCannotMove = board.bishops.getBitboard<side>() & board.getPinnedMaskHV<side>();

    const uint64_t pinnedBishops = board.bishops.getBitboard<side>() & board.getPinnedMaskD12<side>();
    const uint64_t freeBishops = board.bishops.getBitboard<side>() ^ (pinnedBishopsCannotMove | pinnedBishops);

    Utils::runForEachSetBit(pinnedBishops, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Bishop::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getPinnedMaskD12<side>() & board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    Utils::runForEachSetBit(freeBishops, [this, &moveBuilder, &moves](const Square &originSquare) {
        const uint64_t legalMoves = Bishop::getMoves(originSquare, board.getEmptySquares()) &
                                    (board.getOccupiedSquares<Utils::flipColor(side)>() | board.getEmptySquares()) &
                                    board.getCheckMask<side>();

        Utils::runForEachSetBit(legalMoves, [&originSquare, &moveBuilder, &moves](const Square &square) {
            moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(square).withBishop().getMove());
        });
    });

    return moves;
}

#endif // CHESS_ENGINE_LEGALMOVE_INL
