#pragma once

#include "MagicBitboard.h"
#include "MagicBoardGenerator.h"
#include <memory>

class MagicBitboard {
  public:
    std::shared_ptr<MagicBoardGenerator> magicGenerator;
    explicit MagicBitboard(std::shared_ptr<MagicBoardGenerator> &_magicGenerator)
        : magicGenerator{_magicGenerator} {};
};