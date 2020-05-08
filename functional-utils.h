#ifndef ZAUBERON_FUNCTIONAL_UTILS_H
#define ZAUBERON_FUNCTIONAL_UTILS_H

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include "global-types.h"

#define NOW std::chrono::steady_clock::now()
#define ELAPSED_MILLISECONDS(now, then) (std::chrono::duration<TNumber>(now - then).count() * 1000.0)
#define ELAPSED_SECONDS(now, then) std::chrono::duration<TNumber>(now - then).count()
#define ELAPSED_TIME(now, then) std::chrono::duration<TNumber>(now - then).count()

#define BINARY_FUNCTION(function) [](const TNumber n1, const TNumber n2) { return function(n1, n2); }
#define BINARY_FUNCTION_CAPTURE(capture, function)                                                                     \
    [capture](const TNumber n1, const TNumber n2) { return function(n1, n2); }
#define OPEN_FUNCTION(streamName) [](TStream & streamName)
#define OPEN_FUNCTION_CAPTURE(capture, streamName) [capture](TStream & streamName)
#define TERNARY_FUNCTION(function)                                                                                     \
    [](const TNumber n1, const TNumber n2, const TNumber n3) { return function(n1, n2, n3); }
#define TERNARY_FUNCTION_CAPTURE(capture, function)                                                                    \
    [capture](const TNumber n1, const TNumber n2, const TNumber n3) { return function(n1, n2, n3); }
#define UNARY_FUNCTION(function) [](const TNumber n) { return function(n); }
#define UNARY_FUNCTION_CAPTURE(capture, function) [capture](const TNumber n) { return function(n); }

#define FUNCTIONS_H(name)                                                                                              \
    TNumber Binary##name(TNumber n1, TNumber n2) noexcept;                                                             \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    TNumbers name(const TNumber number, const TNS& numbers) noexcept                                                   \
    {                                                                                                                  \
        return Map([number](const TNumber n) { return Binary##name(n, number); }, numbers);                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    TNumbers name(const TNS& numbers1, const TNS& numbers2) noexcept                                                   \
    {                                                                                                                  \
        return Map(Binary##name, numbers1, numbers2);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    TNumbers name(std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2) noexcept                     \
    {                                                                                                                  \
        return Map(Binary##name, numbers1, numbers2);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    TNumber name(const TNS& numbers) noexcept                                                                          \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    TNumber name(std::initializer_list<TN> numbers) noexcept                                                           \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers);                                                                          \
    }

namespace functional_utils
{

using TStream = std::fstream;
using TString = std::string;
using TTimePoint = std::chrono::time_point<std::chrono::steady_clock>;

using TBinaryFunction = std::function<TNumber(const TNumber, const TNumber)>;
using TOpenFunction = std::function<void(TStream&)>;
using TTernaryFunction = std::function<TNumber(const TNumber, const TNumber, const TNumber)>;
using TUnaryFunction = std::function<TNumber(const TNumber)>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
// typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
template <typename T>
auto InternalAlmostEqual(T x, T y, int ulp)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
           // unless the result is subnormal
           || std::fabs(x - y) < std::numeric_limits<T>::min();
}

template <typename TNS>
TNumbers InternalMap(TUnaryFunction uf, const TNS& numbers) noexcept
{
    auto result{TNumbers(numbers.size())};
    std::transform(numbers.begin(), numbers.end(), result.begin(), uf);
    return result;
}

template <typename TNS>
TNumbers InternalMap(TBinaryFunction bf, const TNS& numbers1, const TNS& numbers2) noexcept
{
    auto result{TNumbers(numbers1.size())};
    std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), bf);
    return result;
}

template <typename TNS>
TNumbers InternalMap(TTernaryFunction& tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
{
    auto result{TNumbers(numbers1.size())};
    auto n3{numbers3.begin()};
    std::transform(numbers1.begin(),
                   numbers1.end(),
                   numbers2.begin(),
                   result.begin(),
                   [tf, &n3](const TNumber n1, const TNumber n2) { return tf(n1, n2, *n3++); });
    return result;
}

template <typename TNS>
TNumber InternalReduce(TBinaryFunction bf, const TNS& numbers) noexcept
{
    return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), bf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TTIMES, typename TUF>
void DoTimes(const TTIMES times, TUF uf) noexcept
{
    for (TTIMES i = 0; i < times; ++i)
        uf(i);
}

template <typename TN>
TString FormatElapsedTime(const TN elapsedTime) noexcept
{
    return (elapsedTime >= 1.0) ? std::to_string(elapsedTime) + "s" : std::to_string(1000.0 * elapsedTime) + "ms";
}

template <typename TNS>
TIndex IndexOf(const TNS& numbers, TNumber number)
{
    auto numbersIt{std::find_if(
        numbers.begin(), numbers.end(), [number](const TNumber n) { return InternalAlmostEqual(n, number, 2); })};
    return (numbersIt == numbers.end()) ? -1 : std::distance(numbers.begin(), numbersIt);
}

template <typename TUF, typename TVALUES>
void IMap(TUF uf, TVALUES& values) noexcept
{
    std::transform(values.begin(), values.end(), values.begin(), uf);
}

template <typename TNS>
TNumbers Map(TUnaryFunction uf, const TNS& numbers) noexcept
{
    return InternalMap(uf, numbers);
}

template <typename TN>
TNumbers Map(TUnaryFunction uf, std::initializer_list<TN> numbers) noexcept
{
    return InternalMap(uf, numbers);
}

template <typename TNS>
TNumbers Map(TBinaryFunction bf, const TNS& numbers1, const TNS& numbers2) noexcept
{
    return InternalMap(bf, numbers1, numbers2);
}

template <typename TN>
TNumbers Map(TBinaryFunction bf, std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2) noexcept
{
    return InternalMap(bf, numbers1, numbers2);
}

template <typename TNS>
TNumbers Map(TTernaryFunction tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
{
    return InternalMap(tf, numbers1, numbers2, numbers3);
}

template <typename TN>
TNumbers Map(TTernaryFunction tf,
             std::initializer_list<TN> numbers1,
             std::initializer_list<TN> numbers2,
             std::initializer_list<TN> numbers3) noexcept
{
    return InternalMap(tf, numbers1, numbers2, numbers3);
}

TNumbers Numbers(TIndex count) noexcept;

template <typename TN>
TNumbers Numbers(std::initializer_list<TN> numbers) noexcept
{
    TNumbers result;
    result.insert(result.end(), numbers.begin(), numbers.end());
    return result;
}

template <typename STRING>
void Print(const STRING& s, TStream& os) noexcept
{
    os << s;
}

template <typename STRING>
void PrintLn(const STRING& s, TStream& os) noexcept
{
    os << s << "\n";
}

template <typename STRING>
void Print(const STRING& s) noexcept
{
    Print(s, std::cout);
}

template <typename STRING>
void PrintLn(const STRING& s) noexcept
{
    PrintLn(s, std::cout);
}

template <typename STRING>
void PrintErr(const STRING& s) noexcept
{
    Print(s, std::cerr);
}

template <typename STRING>
void PrintLnErr(const STRING& s) noexcept
{
    PrintLn(s, std::cerr);
}

TNumbers Range(TIndex count) noexcept;
TNumbers Range(TIndex start, TIndex end) noexcept;

template <typename TNS>
TNumber Reduce(TBinaryFunction bf, const TNS& numbers) noexcept
{
    return InternalReduce(bf, numbers);
}

template <typename TN>
TNumber Reduce(TBinaryFunction bf, std::initializer_list<TN> numbers) noexcept
{
    return InternalReduce(bf, numbers);
}

template <typename STRING>
void WithOpen(const STRING& fileSpec, TOpenFunction of) noexcept
{
    auto outputStream{TStream{fileSpec, std::ios::binary}};
    of(outputStream);
}

FUNCTIONS_H(Add)
FUNCTIONS_H(Divide)
FUNCTIONS_H(Multiply)
FUNCTIONS_H(Subtract)

} // namespace functional_utils

#endif // ZAUBERON_FUNCTIONAL_UTILS_H
