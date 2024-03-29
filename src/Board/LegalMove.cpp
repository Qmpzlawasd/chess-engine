#include "LegalMove.h"

template <Color side>
std::vector<Move> LegalMove::getKingLegalMove() const noexcept {
    std::vector<Move> moves;

    uint64_t kingBoard = board.king.getBitboard<side>();
    const auto kingSquare = Utils::popLSB(kingBoard);

    MoveBuilder moveBuilder;
    const uint64_t basicMoves = ~board.computeDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());

    Utils::runForEachSetBit(basicMoves | board.getCastleRightsBitboard<side>(),
                            [&moves, &moveBuilder, &kingSquare, this](const Square &square) noexcept -> void {
                                moves.push_back(moveBuilder.fromSquare(kingSquare).toSquare(square).getMove());
                            });

    return moves;
}

template std::vector<Move> LegalMove::getKingLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getKingLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getRookLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getRookLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getRookLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getPawnLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getPawnLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getPawnLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getQueenLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getQueenLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getQueenLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getKnightLegalMove() const noexcept {
    std::vector<Move> moves;
    const uint64_t freeKnights = board.knights.getBitboard<side>() ^ (board.getPinnedMaskD12<side>() | board.getPinnedMaskHV<side>());
    Utils::showBitBoard(freeKnights);
    MoveBuilder moveBuilder = MoveBuilder{};

    Utils::runForEachSetBit(freeKnights, [&moveBuilder, this, &moves](const Square &originSquare) -> void {
        Utils::runForEachSetBit(Knight::getMoves(originSquare) & ~board.getOccupiedSquares<side>() & board.getCheckMask<side>(),
                                [&moveBuilder, &originSquare, &moves](const Square &legalSquare) -> void {
                                    moves.emplace_back(moveBuilder.fromSquare(originSquare).toSquare(legalSquare).getMove());
                                });
    });
    return moves;
}

template std::vector<Move> LegalMove::getKnightLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getKnightLegalMove<BLACK>() const noexcept;

template <Color side>
std::vector<Move> LegalMove::getBishopLegalMove() const noexcept {
    return std::vector<Move>{};
}

template std::vector<Move> LegalMove::getBishopLegalMove<WHITE>() const noexcept;
template std::vector<Move> LegalMove::getBishopLegalMove<BLACK>() const noexcept;
