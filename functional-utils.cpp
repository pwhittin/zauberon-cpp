#include "functional-utils.h"

#define FUNCTIONS_CPP(name, nameOp)                                                                                    \
    TNumber functional_utils::Binary##name(const TNumber n1, const TNumber n2) noexcept                                \
    {                                                                                                                  \
        return (n1 nameOp n2);                                                                                         \
    }

FUNCTIONS_CPP(Add, +)
FUNCTIONS_CPP(Divide, /)
FUNCTIONS_CPP(Multiply, *)
FUNCTIONS_CPP(Subtract, -)
