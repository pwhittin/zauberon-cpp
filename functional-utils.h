#ifndef ZAUBERON_FUNCTIONAL_UTILS_H
#define ZAUBERON_FUNCTIONAL_UTILS_H

#include <initializer_list>
#include "global-types.h"

#define DEFINE_REDUCER_H(name)                                                                                         \
    TNumber Binary##name(const TNumber n1, const TNumber n2) noexcept;                                                 \
    TNumbers name(const TNumber number, const TNumbers& numbers) noexcept;                                             \
    TNumbers name(const TNumbers& numbers1, const TNumbers& numbers2) noexcept;                                        \
    TNumbers name(std::initializer_list<TNumber> numbers1, std::initializer_list<TNumber> numbers2) noexcept;          \
    TNumber name(const TNumbers& numbers) noexcept;                                                                    \
    TNumber name(std::initializer_list<TNumber> numbers) noexcept;

namespace functional_utils
{

using TUnaryFunction = TNumber (*)(const TNumber);
using TBinaryFunction = TNumber (*)(const TNumber, const TNumber);

TNumbers Map(TUnaryFunction uf, const TNumbers& numbers) noexcept;
TNumbers Map(TUnaryFunction uf, std::initializer_list<TNumber> numbers) noexcept;
TNumbers Map(TBinaryFunction bf, const TNumbers& numbers1, const TNumbers& numbers2) noexcept;
TNumbers
Map(TBinaryFunction bf, std::initializer_list<TNumber> numbers1, std::initializer_list<TNumber> numbers2) noexcept;

TNumbers Numbers(std::initializer_list<TNumber> numbers) noexcept;

TNumbers Range(const unsigned int zeroToThisMinusOne) noexcept;
TNumbers Range(const unsigned int start, const unsigned int endPlusOne) noexcept;

TNumber Reduce(TBinaryFunction bf, const TNumbers& numbers) noexcept;
TNumber Reduce(TBinaryFunction bf, std::initializer_list<TNumber> numbers) noexcept;

DEFINE_REDUCER_H(Add)
DEFINE_REDUCER_H(Divide)
DEFINE_REDUCER_H(Multiply)
DEFINE_REDUCER_H(Subtract)

} // namespace functional_utils

#endif // ZAUBERON_FUNCTIONAL_UTILS_H
