#ifndef CHESS_ENGINE_MATERIAL_H
#define CHESS_ENGINE_MATERIAL_H

#include "Enums/BoardStatus.h"
#include "Enums/Colors.h"
#include "Enums/Points.h"
#include <cstdint>

class Material {
    static constexpr uint8_t MINIMUM_MATERIAL = 3;
    uint8_t materialBlack;
    uint8_t materialWhite;

  public:
    explicit Material(const uint8_t &_materialBlack, const uint8_t &_materialWhite)
        : materialBlack(_materialBlack), materialWhite(_materialWhite) {}

    template <Color side>
    [[nodiscard]] uint8_t getMaterial() const noexcept {
        if constexpr (side == WHITE)
            return materialWhite;
        else
            return materialBlack;
    }

    [[nodiscard]] BoardStatus init() const noexcept {
        if (materialWhite < MINIMUM_MATERIAL and materialBlack < MINIMUM_MATERIAL) {
            return DRAW;
        }
        if (materialWhite < MINIMUM_MATERIAL) {
            return INSUFFICIENT_MATERIAL_WHITE;
        }
        if (materialBlack < MINIMUM_MATERIAL) {
            return INSUFFICIENT_MATERIAL_BLACK;
        }
        return IN_PROGRESS;
    }

    template <Color promotingSide, Points promotedPiece>
    BoardStatus registerPiecePromotion(const BoardStatus &currentStatus) noexcept {
        if constexpr (promotingSide == WHITE) {
            materialWhite -= PAWN_POINTS;
            materialWhite += promotedPiece;
            if (currentStatus == INSUFFICIENT_MATERIAL_BLACK) {
                return materialWhite < MINIMUM_MATERIAL ? DRAW : INSUFFICIENT_MATERIAL_BLACK;
            }
            if (currentStatus == IN_PROGRESS) {
                return materialWhite < MINIMUM_MATERIAL ? INSUFFICIENT_MATERIAL_WHITE : IN_PROGRESS;
            }
        } else {
            materialBlack -= PAWN_POINTS;
            materialBlack += promotedPiece;
            if (currentStatus == INSUFFICIENT_MATERIAL_WHITE) {
                return materialBlack < MINIMUM_MATERIAL ? DRAW : currentStatus;
            }
            if (currentStatus == IN_PROGRESS) {
                return materialBlack < MINIMUM_MATERIAL ? INSUFFICIENT_MATERIAL_BLACK : currentStatus;
            }
        }
        return currentStatus;
    }

    template <Color attackingSide, Points capturedPiece>
    BoardStatus registerPieceCapture(const BoardStatus &currentStatus) noexcept {
        if constexpr (attackingSide == WHITE) {
            materialBlack -= capturedPiece;
            if (currentStatus == INSUFFICIENT_MATERIAL_WHITE) {
                return materialBlack < MINIMUM_MATERIAL ? DRAW : currentStatus;
            }
            if (currentStatus == IN_PROGRESS) {
                return materialBlack < MINIMUM_MATERIAL ? INSUFFICIENT_MATERIAL_BLACK : currentStatus;
            }
        } else {
            materialWhite -= capturedPiece;
            if (currentStatus == INSUFFICIENT_MATERIAL_BLACK) {
                return materialWhite < MINIMUM_MATERIAL ? DRAW : currentStatus;
            }
            if (currentStatus == IN_PROGRESS) {
                return materialWhite < MINIMUM_MATERIAL ? INSUFFICIENT_MATERIAL_WHITE : currentStatus;
            }
        }
        return currentStatus;
    }
};

#endif // CHESS_ENGINE_MATERIAL_H
