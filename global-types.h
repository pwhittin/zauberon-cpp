#ifndef ZAUBERON_GLOBAL_TYPES_H
#define ZAUBERON_GLOBAL_TYPES_H

#include <vector>

#define TArrayNumbers(count) std::array<TNumber, count>
#define TArray(type, count) std::array<type, count>

using TIndex = std::size_t;
using TNumber = double;
using TNumbers = std::vector<TNumber>;

#endif // ZAUBERON_GLOBAL_TYPES_H
