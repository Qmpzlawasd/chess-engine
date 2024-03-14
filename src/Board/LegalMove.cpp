#include "LegalMove.h"

template <Color side>
uint64_t LegalMove::getKingLegalMove() const noexcept {
    const uint64_t basicMoves = ~board.getDangerTable<side>() & King::getMoves(board.king.getBitboard<side>());

    return basicMoves | board.getCastleRightsBitboard<side>();
}

template uint64_t LegalMove::getKingLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getKingLegalMove<BLACK>() const noexcept;

template <Color side>
uint64_t LegalMove::getRookLegalMove() const noexcept {
    return 0;
}

template uint64_t LegalMove::getRookLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getRookLegalMove<BLACK>() const noexcept;

template <Color side>
uint64_t LegalMove::getPawnLegalMove() const noexcept {
    return 0;
}

template uint64_t LegalMove::getPawnLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getPawnLegalMove<BLACK>() const noexcept;

template <Color side>
uint64_t LegalMove::getQueenLegalMove() const noexcept {
    return 0;
}

template uint64_t LegalMove::getQueenLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getQueenLegalMove<BLACK>() const noexcept;

template <Color side>
uint64_t LegalMove::getKnightLegalMove() const noexcept {
    return 0;
}

template uint64_t LegalMove::getKnightLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getKnightLegalMove<BLACK>() const noexcept;

template <Color side>
uint64_t LegalMove::getBishopLegalMove() const noexcept {
    return 0;
}
template uint64_t LegalMove::getBishopLegalMove<WHITE>() const noexcept;
template uint64_t LegalMove::getBishopLegalMove<BLACK>() const noexcept;
