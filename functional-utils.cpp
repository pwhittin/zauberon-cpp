#include <algorithm>
#include <functional>
#include <numeric>
#include "functional-utils.h"

#define DEFINE_REDUCER_CPP(name, op)                                                                                   \
    TNumber functional_utils::Binary##name(const TNumber n1, const TNumber n2) noexcept                                \
    {                                                                                                                  \
        return (n1 op n2);                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    TNumbers functional_utils::name(const TNumbers& numbers1, const TNumbers& numbers2) noexcept                       \
    {                                                                                                                  \
        return functional_utils::Map(Binary##name, numbers1, numbers2);                                                \
    }                                                                                                                  \
                                                                                                                       \
    TNumbers functional_utils::name(std::initializer_list<TNumber> numbers1,                                           \
                                    std::initializer_list<TNumber> numbers2) noexcept                                  \
    {                                                                                                                  \
        auto result{TNumbers(std::max(numbers1.size(), numbers2.size()))};                                             \
        std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), Binary##name);              \
        return result;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    TNumber functional_utils::name(const TNumbers& numbers) noexcept                                                   \
    {                                                                                                                  \
        return functional_utils::Reduce(Binary##name, numbers);                                                        \
    }                                                                                                                  \
                                                                                                                       \
    TNumber functional_utils::name(std::initializer_list<TNumber> numbers) noexcept                                    \
    {                                                                                                                  \
        return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), std::ptr_fun(Binary##name));      \
    }

TNumbers functional_utils::Numbers(std::initializer_list<TNumber> numbers) noexcept
{
    TNumbers result;
    result.insert(result.end(), numbers.begin(), numbers.end());
    return result;
}

TNumbers functional_utils::Map(TUnaryFunction uf, const TNumbers& numbers) noexcept
{
    auto result{TNumbers(numbers.size())};
    std::transform(numbers.cbegin(), numbers.cend(), result.begin(), uf);
    return result;
}

TNumbers functional_utils::Map(TUnaryFunction uf, std::initializer_list<TNumber> numbers) noexcept
{
    auto result{TNumbers(numbers.size())};
    std::transform(numbers.begin(), numbers.end(), result.begin(), uf);
    return result;
}

TNumbers functional_utils::Map(TBinaryFunction bf, const TNumbers& numbers1, const TNumbers& numbers2) noexcept
{
    auto result{TNumbers(numbers1.size())};
    std::transform(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), result.begin(), bf);
    return result;
}

TNumbers functional_utils::Map(TBinaryFunction bf,
                               std::initializer_list<TNumber> numbers1,
                               std::initializer_list<TNumber> numbers2) noexcept
{
    auto result{TNumbers(std::max(numbers1.size(), numbers1.size()))};
    std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), bf);
    return result;
}

TNumber functional_utils::Reduce(TBinaryFunction bf, const TNumbers& numbers) noexcept
{
    return std::accumulate(numbers.cbegin() + 1, numbers.cend(), *numbers.cbegin(), bf);
}

TNumber functional_utils::Reduce(TBinaryFunction bf, std::initializer_list<TNumber> numbers) noexcept
{
    return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), bf);
}

DEFINE_REDUCER_CPP(Add, +)
DEFINE_REDUCER_CPP(Divide, /)
DEFINE_REDUCER_CPP(Multiply, *)
DEFINE_REDUCER_CPP(Subtract, -)
