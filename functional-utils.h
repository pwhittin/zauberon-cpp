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
    void name(const TNumber number, const TNS& numbers, TNS& numbersDest) noexcept                                     \
    {                                                                                                                  \
        Map([number](const TNumber n) { return Binary##name(n, number); }, numbers, numbersDest);                      \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    void name(const TNS& numbers1, const TNS& numbers2, TNS& numbersDest) noexcept                                     \
    {                                                                                                                  \
        Map(Binary##name, numbers1, numbers2, numbersDest);                                                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    void name(std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2, TNS& numbersDest) noexcept       \
    {                                                                                                                  \
        Map(Binary##name, numbers1, numbers2, numbersDest);                                                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    void name(std::initializer_list<TN> numbers1, const TNS& numbers2, TNS& numbersDest) noexcept                      \
    {                                                                                                                  \
        Map(Binary##name, numbers1, numbers2, numbersDest);                                                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    void name(const TNS& numbers1, std::initializer_list<TN> numbers2, TNS& numbersDest) noexcept                      \
    {                                                                                                                  \
        Map(Binary##name, numbers1, numbers2, numbersDest);                                                            \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS, typename TN>                                                                               \
    void name(const TNS& numbers, TN& answer) noexcept                                                                 \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers, answer);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    auto name(std::initializer_list<TN> numbers, TN& answer) noexcept                                                  \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers, answer);                                                                  \
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

template <typename TUF, typename TVS>
void InternalMap(TUF uf, const TVS& values, TVS& valuesDest) noexcept
{
    std::transform(values.begin(), values.end(), valuesDest.begin(), uf);
}

template <typename TBF, typename TVS>
void InternalMap(TBF bf, const TVS& values1, const TVS& values2, TVS& valuesDest) noexcept
{
    std::transform(values1.begin(), values1.end(), values2.begin(), valuesDest.begin(), bf);
}

template <typename TTF, typename TNS>
void InternalMap(TTF& tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3, TNS& numbersDest) noexcept
{
    auto n3{numbers3.begin()};
    std::transform(numbers1.begin(),
                   numbers1.end(),
                   numbers2.begin(),
                   numbersDest.begin(),
                   [tf, &n3](const TNumber n1, const TNumber n2) { return tf(n1, n2, *n3++); });
}

template <typename TBF, typename TVS, typename TV>
void InternalReduce(TBF bf, const TVS& values, TV& value) noexcept
{
    value = std::accumulate(values.begin() + 1, values.end(), *values.begin(), bf);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// external
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

template <typename TVS, typename T>
auto IndexOf(const TVS& values, T value)
{
    auto valuesIt{
        std::find_if(values.begin(), values.end(), [value](const T n) { return InternalAlmostEqual(n, value, 2); })};
    return (valuesIt == values.end()) ? -1 : std::distance(values.begin(), valuesIt);
}

template <typename TUF, typename TVALUES>
void Map(TUF uf, const TVALUES& values, TVALUES& valuesDest) noexcept
{
    InternalMap(uf, values, valuesDest);
}

template <typename TUF, typename TV, typename TVS>
void Map(TUF uf, std::initializer_list<TV> values, TVS& valuesDest) noexcept
{
    InternalMap(uf, values, valuesDest);
}

template <typename TBF, typename TVS>
void Map(TBF bf, const TVS& values1, const TVS& values2, TVS& valuesDest) noexcept
{
    InternalMap(bf, values1, values2, valuesDest);
}

template <typename TBF, typename TV, typename TVS>
void Map(TBF bf, std::initializer_list<TV> values1, std::initializer_list<TV> values2, TVS& valuesDest) noexcept
{
    InternalMap(bf, values1, values2, valuesDest);
}

template <typename TBF, typename TV, typename TVS>
void Map(TBF bf, std::initializer_list<TV> values1, const TVS& values2, TVS& valuesDest) noexcept
{
    InternalMap(bf, values1, values2, valuesDest);
}

template <typename TBF, typename TV, typename TVS>
void Map(TBF bf, const TVS& values1, std::initializer_list<TV> values2, TVS& valuesDest) noexcept
{
    InternalMap(bf, values1, values2, valuesDest);
}

template <typename TTF, typename TVS>
void Map(TTF tf, const TVS& values1, const TVS& values2, const TVS& values3, TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf,
         std::initializer_list<TV> values1,
         std::initializer_list<TV> values2,
         std::initializer_list<TV> values3,
         TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf,
         std::initializer_list<TV> values1,
         std::initializer_list<TV> values2,
         TVS& values3,
         TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf,
         std::initializer_list<TV> values1,
         TVS& values2,
         std::initializer_list<TV> values3,
         TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf,
         TVS& values1,
         std::initializer_list<TV> values2,
         std::initializer_list<TV> values3,
         TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf, std::initializer_list<TV> values1, TVS& values2, TVS& values3, TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf, TVS& values1, std::initializer_list<TV> values2, TVS& values3, TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TTF, typename TV, typename TVS>
void Map(TTernaryFunction tf, TVS& values1, TVS& values2, std::initializer_list<TV> values3, TVS& valuesDest) noexcept
{
    InternalMap(tf, values1, values2, values3, valuesDest);
}

template <typename TV, typename TVS>
void Values(std::initializer_list<TV> values, TVS& valuesDest) noexcept
{
    std::copy(values.begin(), values.end(), valuesDest.begin());
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

template <typename I, typename TNS>
void Range(const I start, TNS& values) noexcept
{
    std::iota(values.begin(), values.end(), start);
}

template <typename TNS>
void Range(TNS& values) noexcept
{
    Range(0, values);
}

template <typename TBF, typename TVS, typename TV>
void Reduce(TBF bf, const TVS& values, TV& value) noexcept
{
    InternalReduce(bf, values, value);
}

template <typename TBF, typename TV>
void Reduce(TBF bf, std::initializer_list<TV> values, TV& value) noexcept
{
    InternalReduce(bf, values, value);
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
