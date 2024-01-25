#pragma once
#include "MagicBoardGenerator.cpp"
#include "Piece.cpp"
#include "Utils.cpp"
#include <memory>

class MagicBitboard {
  public:
    std::shared_ptr<MagicBoardGenerator> magicGenerator;
    MagicBitboard(std::shared_ptr<MagicBoardGenerator> &_magicGenerator)
        : magicGenerator{_magicGenerator} {};
};