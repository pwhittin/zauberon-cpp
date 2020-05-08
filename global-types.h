#ifndef ZAUBERON_GLOBAL_TYPES_H
#define ZAUBERON_GLOBAL_TYPES_H

#include <array>

#define TArray(type, count) std::array<type, count>
#define TNumbers(count) std::array<TNumber, count>

using TIndex = std::size_t;
using TNumber = double;

#endif // ZAUBERON_GLOBAL_TYPES_H
