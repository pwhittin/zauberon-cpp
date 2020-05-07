#include <algorithm>
#include <functional>
#include <numeric>
#include "functional-utils.h"

#define DEFINE_REDUCER_CPP(name, nameOp)                                                                               \
    TNumber functional_utils::Binary##name(const TNumber n1, const TNumber n2) noexcept                                \
    {                                                                                                                  \
        return (n1 nameOp n2);                                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    TNumbers functional_utils::name(const TNumber number, const TNumbers& numbers) noexcept                            \
    {                                                                                                                  \
        return TMap([number](const TNumber n) { return Binary##name(n, number); }, numbers);                           \
    }                                                                                                                  \
                                                                                                                       \
    TNumbers functional_utils::name(const TNumbers& numbers1, const TNumbers& numbers2) noexcept                       \
    {                                                                                                                  \
        return TMap(Binary##name, numbers1, numbers2);                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    TNumbers functional_utils::name(std::initializer_list<TNumber> numbers1,                                           \
                                    std::initializer_list<TNumber> numbers2) noexcept                                  \
    {                                                                                                                  \
        return TMap(Binary##name, numbers1, numbers2);                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    TNumber functional_utils::name(const TNumbers& numbers) noexcept                                                   \
    {                                                                                                                  \
        return TReduce(Binary##name, numbers);                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    TNumber functional_utils::name(std::initializer_list<TNumber> numbers) noexcept                                    \
    {                                                                                                                  \
        return TReduce(Binary##name, numbers);                                                                         \
    }

TNumbers functional_utils::Numbers(std::initializer_list<TNumber> numbers) noexcept
{
    TNumbers result;
    result.insert(result.end(), numbers.begin(), numbers.end());
    return result;
}

template <typename TUF, typename TNS>
TNumbers TMap(TUF uf, const TNS& numbers) noexcept
{
    auto result{TNumbers(numbers.size())};
    std::transform(numbers.begin(), numbers.end(), result.begin(), uf);
    return result;
}

TNumbers functional_utils::Map(TUnaryFunction uf, const TNumbers& numbers) noexcept
{
    return TMap(uf, numbers);
}

TNumbers functional_utils::Map(TUnaryFunction uf, std::initializer_list<TNumber> numbers) noexcept
{
    return TMap(uf, numbers);
}

template <typename TBF, typename TNS>
TNumbers TMap(TBF bf, const TNS& numbers1, const TNS& numbers2) noexcept
{
    auto result{TNumbers(numbers1.size())};
    std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), bf);
    return result;
}

TNumbers functional_utils::Map(TBinaryFunction bf, const TNumbers& numbers1, const TNumbers& numbers2) noexcept
{
    return TMap(bf, numbers1, numbers2);
}

TNumbers functional_utils::Map(TBinaryFunction bf,
                               std::initializer_list<TNumber> numbers1,
                               std::initializer_list<TNumber> numbers2) noexcept
{
    return TMap(bf, numbers1, numbers2);
}

template <typename TTF, typename TNS>
TNumbers TMap(TTF tf, const TNS& numbers1, const TNS& numbers2, const TNS& numbers3) noexcept
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

TNumbers functional_utils::Map(TTernaryFunction tf,
                               const TNumbers& numbers1,
                               const TNumbers& numbers2,
                               const TNumbers& numbers3) noexcept
{
    return TMap(tf, numbers1, numbers2, numbers3);
}

TNumbers functional_utils::Map(TTernaryFunction tf,
                               std::initializer_list<TNumber> numbers1,
                               std::initializer_list<TNumber> numbers2,
                               std::initializer_list<TNumber> numbers3) noexcept
{
    return TMap(tf, numbers1, numbers2, numbers3);
}

TNumbers functional_utils::Range(const unsigned int zeroToThisMinusOne) noexcept
{
    return Range(0, zeroToThisMinusOne);
}

TNumbers functional_utils::Range(const unsigned int start, const unsigned int endPlusOne) noexcept
{
    auto result{TNumbers(endPlusOne - start)};
    std::iota(result.begin(), result.end(), start);
    return result;
}

template <typename TBF, typename TNS>
TNumber TReduce(TBF bf, const TNS& numbers) noexcept
{
    return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), bf);
}

TNumber functional_utils::Reduce(TBinaryFunction bf, const TNumbers& numbers) noexcept
{
    return TReduce(bf, numbers);
}

TNumber functional_utils::Reduce(TBinaryFunction bf, std::initializer_list<TNumber> numbers) noexcept
{
    return TReduce(bf, numbers);
}

DEFINE_REDUCER_CPP(Add, +)
DEFINE_REDUCER_CPP(Divide, /)
DEFINE_REDUCER_CPP(Multiply, *)
DEFINE_REDUCER_CPP(Subtract, -)
