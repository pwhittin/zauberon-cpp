#include "functional-utils.h"

#define FUNCTIONS_CPP(name, nameOp)                                                                                    \
    TNumber functional_utils::Binary##name(const TNumber n1, const TNumber n2) noexcept                                \
    {                                                                                                                  \
        return (n1 nameOp n2);                                                                                         \
    }

TNumbers functional_utils::Numbers(const TIndex count) noexcept
{
    return TNumbers(count);
}

TNumbers functional_utils::Range(const TIndex count) noexcept
{
    return Range(0, count);
}

TNumbers functional_utils::Range(const TIndex start, const TIndex end) noexcept
{
    auto result{TNumbers(end - start)};
    std::iota(result.begin(), result.end(), start);
    return result;
}

FUNCTIONS_CPP(Add, +)
FUNCTIONS_CPP(Divide, /)
FUNCTIONS_CPP(Multiply, *)
FUNCTIONS_CPP(Subtract, -)
