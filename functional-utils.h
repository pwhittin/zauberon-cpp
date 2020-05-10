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
    auto name(TNS& numbersDest, const TNumber number, const TNS& numbers) noexcept                                     \
    {                                                                                                                  \
        return Map(numbersDest, [number](const TNumber n) { return Binary##name(n, number); }, numbers);               \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS>                                                                                            \
    auto name(TNS& numbersDest, const TNS& numbers1, const TNS& numbers2) noexcept                                     \
    {                                                                                                                  \
        return Map(numbersDest, Binary##name, numbers1, numbers2);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    auto name(TNS& numbersDest, std::initializer_list<TN> numbers1, std::initializer_list<TN> numbers2) noexcept       \
    {                                                                                                                  \
        return Map(numbersDest, Binary##name, numbers1, numbers2);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    auto name(TNS& numbersDest, std::initializer_list<TN> numbers1, const TNS& numbers2) noexcept                      \
    {                                                                                                                  \
        return Map(numbersDest, Binary##name, numbers1, numbers2);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN, typename TNS>                                                                               \
    auto name(TNS& numbersDest, const TNS& numbers1, std::initializer_list<TN> numbers2) noexcept                      \
    {                                                                                                                  \
        return Map(numbersDest, Binary##name, numbers1, numbers2);                                                     \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TNS, typename TN>                                                                               \
    auto name(TN& answer, const TNS& numbers) noexcept                                                                 \
    {                                                                                                                  \
        return Reduce(answer, Binary##name, numbers);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    template <typename TN>                                                                                             \
    auto name(TN& answer, std::initializer_list<TN> numbers) noexcept                                                  \
    {                                                                                                                  \
        return Reduce(answer, Binary##name, numbers);                                                                  \
    }

namespace functional_utils
{

using TStream = std::ofstream;
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

template <typename TV, typename TBF, typename TVS>
auto InternalReduce(TV& value, TBF bf, const TVS& values) noexcept
{
    value = std::accumulate(values.begin() + 1, values.end(), *values.begin(), bf);
    return value;
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

template <typename TUF, typename TVS>
auto Map(TVS& valuesDest, TUF uf, const TVS& values) noexcept
{
    std::transform(values.begin(), values.end(), valuesDest.begin(), uf);
    return valuesDest;
}

template <typename TUF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TUF uf, std::initializer_list<TV> values) noexcept
{
    InternalMap(valuesDest, uf, values);
    return valuesDest;
}

template <typename TBF, typename TVS>
auto Map(TVS& valuesDest, TBF bf, const TVS& values1, const TVS& values2) noexcept
{
    std::transform(values1.begin(), values1.end(), values2.begin(), valuesDest.begin(), bf);
    return valuesDest;
}

template <typename TBF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TBF bf, std::initializer_list<TV> values1, std::initializer_list<TV> values2) noexcept
{
    std::transform(values1.begin(), values1.end(), values2.begin(), valuesDest.begin(), bf);
    return valuesDest;
}

template <typename TBF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TBF bf, std::initializer_list<TV> values1, const TVS& values2) noexcept
{
    InternalMap(valuesDest, bf, values1, values2);
    return valuesDest;
}

template <typename TBF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TBF bf, const TVS& values1, std::initializer_list<TV> values2) noexcept
{
    InternalMap(valuesDest, bf, values1, values2);
    return valuesDest;
}

template <typename TTF, typename TNS>
auto Map(TNS& numbersDest, TTF& tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
{
    auto n3{numbers3.begin()};
    std::transform(numbers1.begin(),
                   numbers1.end(),
                   numbers2.begin(),
                   numbersDest.begin(),
                   [tf, &n3](const TNumber n1, const TNumber n2) { return tf(n1, n2, *n3++); });
    return numbersDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest,
         TTernaryFunction tf,
         std::initializer_list<TV> values1,
         std::initializer_list<TV> values2,
         std::initializer_list<TV> values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest,
         TTernaryFunction tf,
         std::initializer_list<TV> values1,
         std::initializer_list<TV> values2,
         TVS& values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest,
         TTernaryFunction tf,
         std::initializer_list<TV> values1,
         TVS& values2,
         std::initializer_list<TV> values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest,
         TTernaryFunction tf,
         TVS& values1,
         std::initializer_list<TV> values2,
         std::initializer_list<TV> values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TTernaryFunction tf, std::initializer_list<TV> values1, TVS& values2, TVS& values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TTernaryFunction tf, TVS& values1, std::initializer_list<TV> values2, TVS& values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
}

template <typename TTF, typename TV, typename TVS>
auto Map(TVS& valuesDest, TTernaryFunction tf, TVS& values1, TVS& values2, std::initializer_list<TV> values3) noexcept
{
    InternalMap(valuesDest, tf, values1, values2, values3);
    return valuesDest;
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

template <typename TNS, typename I>
auto Range(TNS& values, const I start) noexcept
{
    std::iota(values.begin(), values.end(), start);
    return values;
}

template <typename TNS>
auto Range(TNS& values) noexcept
{
    Range(values, 0);
    return values;
}

template <typename TV, typename TBF, typename TVS>
auto Reduce(TV& value, TBF bf, const TVS& values) noexcept
{
    return InternalReduce(value, bf, values);
}

template <typename TBF, typename TV>
auto Reduce(TV& value, TBF bf, std::initializer_list<TV> values) noexcept
{
    return InternalReduce(value, bf, values);
}

template <typename TV, typename TVS>
auto Values(TVS& valuesDest, std::initializer_list<TV> values) noexcept
{
    std::copy(values.begin(), values.end(), valuesDest.begin());
    return valuesDest;
}

template <typename STRING>
void WithOpen(const STRING& fileSpec, TOpenFunction of) noexcept
{
    auto outputStream{TStream{fileSpec, std::ios::binary}};
    of(outputStream);
    outputStream.flush();
    outputStream.close();
}

FUNCTIONS_H(Add)
FUNCTIONS_H(Divide)
FUNCTIONS_H(Multiply)
FUNCTIONS_H(Subtract)

} // namespace functional_utils

#endif // ZAUBERON_FUNCTIONAL_UTILS_H
