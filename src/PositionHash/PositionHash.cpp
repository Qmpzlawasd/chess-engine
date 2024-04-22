#include "PositionHash.h"

std::function<uint64_t()> PositionHash::generator = getGeneratorFunc();
