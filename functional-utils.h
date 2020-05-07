#ifndef ZAUBERON_FUNCTIONAL_UTILS_H
#define ZAUBERON_FUNCTIONAL_UTILS_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include "global-types.h"

#define DEFINE_REDUCER_H(name)                                                                                         \
    TNumber Binary##name(TNumber n1, TNumber n2) noexcept;                                                             \
    TNumbers name(TNumber number, const TNumbers& numbers) noexcept;                                                   \
    TNumbers name(const TNumbers& numbers1, const TNumbers& numbers2) noexcept;                                        \
    TNumbers name(std::initializer_list<TNumber> numbers1, std::initializer_list<TNumber> numbers2) noexcept;          \
    TNumber name(const TNumbers& numbers) noexcept;                                                                    \
    TNumber name(std::initializer_list<TNumber> numbers) noexcept;

namespace functional_utils
{

using TOutput = std::ofstream;

using TBinaryFunction = TNumber (*)(const TNumber, const TNumber);
using TOpenFunction = std::function<void(TOutput&)>;
using TTernaryFunction = TNumber (*)(const TNumber, const TNumber, const TNumber);
using TUnaryFunction = TNumber (*)(const TNumber);

template <typename TTIMES, typename TUF>
void DoTimes(const TTIMES times, TUF uf) noexcept
{
    for (TTIMES i = 0; i < times; ++i)
        uf(i);
}

TIndex IndexOf(const TNumbers& numbers, TNumber number);

template <typename TUF, typename TVALUES>
void IMap(TUF uf, TVALUES& values) noexcept
{
    std::transform(values.begin(), values.end(), values.begin(), uf);
}

TNumbers Map(TUnaryFunction uf, const TNumbers& numbers) noexcept;
TNumbers Map(TUnaryFunction uf, std::initializer_list<TNumber> numbers) noexcept;
TNumbers Map(TBinaryFunction bf, const TNumbers& numbers1, const TNumbers& numbers2) noexcept;
TNumbers
Map(TBinaryFunction bf, std::initializer_list<TNumber> numbers1, std::initializer_list<TNumber> numbers2) noexcept;
TNumbers
Map(TTernaryFunction tf, const TNumbers& numbers1, const TNumbers& numbers2, const TNumbers& numbers3) noexcept;
TNumbers Map(TTernaryFunction tf,
             std::initializer_list<TNumber> numbers1,
             std::initializer_list<TNumber> numbers2,
             std::initializer_list<TNumber> numbers3) noexcept;

TNumbers Numbers(std::initializer_list<TNumber> numbers) noexcept;

template <typename STRING>
void PrintString(const STRING& s, TOutput& os) noexcept
{
    os << s;
}

template <typename STRING>
void PrintLnString(const STRING& s, TOutput& os) noexcept
{
    os << s << "\n";
}

template <typename STRING>
void PrintString(const STRING& s) noexcept
{
    PrintString(s, std::cout);
}

template <typename STRING>
void PrintLnString(const STRING& s) noexcept
{
    PrintLnString(s, std::cout);
}

template <typename STRING>
void PrintErrString(const STRING& s) noexcept
{
    PrintString(s, std::cerr);
}

template <typename STRING>
void PrintErrLnString(const STRING& s) noexcept
{
    PrintLnString(s, std::cerr);
}

TNumbers Range(unsigned int zeroToThisMinusOne) noexcept;
TNumbers Range(unsigned int start, unsigned int endPlusOne) noexcept;

TNumber Reduce(TBinaryFunction bf, const TNumbers& numbers) noexcept;
TNumber Reduce(TBinaryFunction bf, std::initializer_list<TNumber> numbers) noexcept;

template <typename STRING>
void WithOpen(const STRING& fileSpec, TOpenFunction of) noexcept
{
    auto outputStream{TOutput{fileSpec, std::ios::binary}};
    of(outputStream);
}

DEFINE_REDUCER_H(Add)
DEFINE_REDUCER_H(Divide)
DEFINE_REDUCER_H(Multiply)
DEFINE_REDUCER_H(Subtract)

} // namespace functional_utils

#endif // ZAUBERON_FUNCTIONAL_UTILS_H
