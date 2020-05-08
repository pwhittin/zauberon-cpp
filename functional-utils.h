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
#include <string>
#include "global-types.h"

#define NOW std::chrono::steady_clock::now()
#define ELAPSED_MILLISECONDS(now, then) (std::chrono::duration<TNumber>(now - then).count() * 1000.0)
#define ELAPSED_SECONDS(now, then) std::chrono::duration<TNumber>(now - then).count()
#define ELAPSED_TIME(now, then) std::chrono::duration<TNumber>(now - then).count()

#define BINARY_FN(function)                                                                                            \
    (const TNumber n1, const TNumber n2)                                                                               \
    {                                                                                                                  \
        return function(n1, n2);                                                                                       \
    }

#define TERNARY_FN(function)                                                                                           \
    (const TNumber n1, const TNumber n2, const TNumber n3)                                                             \
    {                                                                                                                  \
        return function(n1, n2, n3);                                                                                   \
    }

#define UNARY_FN(function)                                                                                             \
    (const TNumber n)                                                                                                  \
    {                                                                                                                  \
        return function(n);                                                                                            \
    }

#define UNARY_TYPE_NAME(type, name) (const type name)

#define DOTIMES_FN(index) (const TIndex index)
#define OPEN_FN(streamName) (TStream & streamName)

#define FUNCTIONS_H(name)                                                                                              \
    TNumber Binary##name(TNumber n1, TNumber n2) noexcept;                                                             \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    auto name(const TNumber number, const TNS& numbers) noexcept                                                       \
    {                                                                                                                  \
        return Map([number](const TNumber n) { return Binary##name(n, number); }, numbers);                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    auto name(const TNS& numbers1, const TNS& numbers2) noexcept                                                       \
    {                                                                                                                  \
        return Map(Binary##name, numbers1, numbers2);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    auto name(std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2) noexcept                         \
    {                                                                                                                  \
        return Map(Binary##name, numbers1, numbers2);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    auto name(const TNS& numbers) noexcept                                                                             \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    auto name(std::initializer_list<TN> numbers) noexcept                                                              \
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
auto InternalMap(TUnaryFunction uf, const TNS& numbers) noexcept
{
    auto result{TNumbers(numbers.size())};
    std::transform(numbers.begin(), numbers.end(), result.begin(), uf);
    return result;
}

template <typename TNS>
auto InternalMap(TBinaryFunction bf, const TNS& numbers1, const TNS& numbers2) noexcept
{
    auto result{TNumbers(numbers1.size())};
    std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), bf);
    return result;
}

template <typename TNS>
auto InternalMap(TTernaryFunction& tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
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
auto InternalReduce(TBinaryFunction bf, const TNS& numbers) noexcept
{
    return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), bf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNS>
auto Count(const TNS& ns) noexcept
{
    return ns.size();
}

template <typename TTIMES, typename TUF>
void DoTimes(const TTIMES times, TUF uf) noexcept
{
    for (TTIMES i = 0; i < times; ++i)
        uf(i);
}

template <typename TN>
auto FormatElapsedTime(const TN elapsedTime) noexcept
{
    return (elapsedTime >= 1.0) ? std::to_string(elapsedTime) + "s" : std::to_string(1000.0 * elapsedTime) + "ms";
}

template <typename TNS>
auto FormatNumbers(const TNS& numbers) noexcept
{
    static const auto MAX_WIDTH(80);
    auto result{std::string("")};
    auto lineLength{result.length()};
    for (auto i : numbers)
    {
        auto iStr{std::to_string(i) + " "};
        if ((lineLength + iStr.length()) > MAX_WIDTH)
        {
            iStr = "\n" + iStr;
            lineLength = 0;
        }
        result += iStr;
        lineLength += iStr.length();
    }
    result.pop_back();
    return result;
}

template <typename TNS>
auto IndexOf(const TNS& numbers, TNumber number)
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
auto Map(TUnaryFunction uf, const TNS& numbers) noexcept
{
    return InternalMap(uf, numbers);
}

template <typename TN>
auto Map(TUnaryFunction uf, std::initializer_list<TN> numbers) noexcept
{
    return InternalMap(uf, numbers);
}

template <typename TNS>
auto Map(TBinaryFunction bf, const TNS& numbers1, const TNS& numbers2) noexcept
{
    return InternalMap(bf, numbers1, numbers2);
}

template <typename TN>
auto Map(TBinaryFunction bf, std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2) noexcept
{
    return InternalMap(bf, numbers1, numbers2);
}

template <typename TNS>
auto Map(TTernaryFunction tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
{
    return InternalMap(tf, numbers1, numbers2, numbers3);
}

template <typename TN>
auto Map(TTernaryFunction tf,
         std::initializer_list<TN> numbers1,
         std::initializer_list<TN> numbers2,
         std::initializer_list<TN> numbers3) noexcept
{
    return InternalMap(tf, numbers1, numbers2, numbers3);
}

template <typename I>
auto Numbers(const I count) noexcept
{
    return TNumbers(count);
}

template <typename TN>
auto Numbers(std::initializer_list<TN> numbers) noexcept
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
    std::cout << s;
}

template <typename STRING>
void PrintLn(const STRING& s) noexcept
{
    std::cout << s << "\n";
}

template <typename STRING>
void PrintErr(const STRING& s) noexcept
{
    std::cerr << s;
}

template <typename STRING>
void PrintLnErr(const STRING& s) noexcept
{
    std::cerr << s << "\n";
}

template <typename I>
auto Range(const I start, const I end) noexcept
{
    auto result{TNumbers(end - start)};
    std::iota(result.begin(), result.end(), start);
    return result;
}

template <typename I>
auto Range(const I count) noexcept
{
    return Range(0, count);
}

template <typename TNS>
auto Reduce(TBinaryFunction bf, const TNS& numbers) noexcept
{
    return InternalReduce(bf, numbers);
}

template <typename TN>
auto Reduce(TBinaryFunction bf, std::initializer_list<TN> numbers) noexcept
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
