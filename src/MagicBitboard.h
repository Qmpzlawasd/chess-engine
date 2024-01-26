#pragma once

#include "MagicBitboard.h"
#include "MagicValuesGeneratorInterface.h"
#include <memory>

class MagicBitboard {
  public:
    std::shared_ptr<MagicValuesGeneratorInterface> magicGenerator;
    explicit MagicBitboard(std::shared_ptr<MagicValuesGeneratorInterface> &_magicGenerator) : magicGenerator{_magicGenerator} {};
};