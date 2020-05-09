#include <array>
#include "../functional-utils.h"
#include "catch.hpp"

using namespace functional_utils;

TEST_CASE("IndexOf", "[IndexOfTest]")
{
    SECTION("one number")
    {
        TNumbers(1) numbers;
        Values(numbers, {11.1});
        CHECK(IndexOf(numbers, 11.1) == 0);
        CHECK(IndexOf(numbers, 11.2) == -1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers;
        Values(numbers, {11.1, 22.2, 33.3});
        CHECK(IndexOf(numbers, 11.1) == 0);
        CHECK(IndexOf(numbers, 22.2) == 1);
        CHECK(IndexOf(numbers, 33.3) == 2);
        CHECK(IndexOf(numbers, 44.4) == -1);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers;
        Values(numbers, {11.1, 22.2, 33.3, 44.4, 55.5});
        CHECK(IndexOf(numbers, 11.1) == 0);
        CHECK(IndexOf(numbers, 22.2) == 1);
        CHECK(IndexOf(numbers, 33.3) == 2);
        CHECK(IndexOf(numbers, 44.4) == 3);
        CHECK(IndexOf(numbers, 55.5) == 4);
        CHECK(IndexOf(numbers, 66.6) == -1);
    }
}

TEST_CASE("Map Unary Numbers", "[MapUnaryNumbersTest]")
{
    auto uf = [](const TNumber n) { return (1 + n); };

    SECTION("one number")
    {
        TNumbers(1) numbers;
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers;
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1, 22.2, 33.3}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers[1] == Approx(23.2));
        CHECK(mappedNumbers[2] == Approx(34.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers;
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1, 22.2, 33.3, 44.4, 55.5}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers[1] == Approx(23.2));
        CHECK(mappedNumbers[2] == Approx(34.3));
        CHECK(mappedNumbers[3] == Approx(45.4));
        CHECK(mappedNumbers[4] == Approx(56.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Map Unary Inline", "[MapUnaryInlineTest]")
{
    auto uf = [](const TNumber n) { return (1 + n); };

    SECTION("one number")
    {
        TNumbers(1) numbers;
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers;
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1, 22.2, 33.3}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers[1] == Approx(23.2));
        CHECK(mappedNumbers[2] == Approx(34.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers;
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers, uf, Values(numbers, {11.1, 22.2, 33.3, 44.4, 55.5}));
        CHECK(mappedNumbers[0] == Approx(12.1));
        CHECK(mappedNumbers[1] == Approx(23.2));
        CHECK(mappedNumbers[2] == Approx(34.3));
        CHECK(mappedNumbers[3] == Approx(45.4));
        CHECK(mappedNumbers[4] == Approx(56.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Map Binary Numbers", "[MapBinaryNumbersTest]")
{
    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };

    SECTION("one number")
    {
        TNumbers(1) numbers1;
        TNumbers(1) numbers2;
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, bf, Values(numbers1, {11.1}), Values(numbers2, {10.0}));
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers1;
        TNumbers(3) numbers2;
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers, bf, Values(numbers1, {11.1, 22.2, 33.3}), Values(numbers2, {10.0, 20.0, 30.0}));
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers[1] == Approx(42.2));
        CHECK(mappedNumbers[2] == Approx(63.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers1;
        TNumbers(5) numbers2;
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers,
            bf,
            Values(numbers1, {11.1, 22.2, 33.3, 44.4, 55.5}),
            Values(numbers2, {10.0, 20.0, 30.0, 40.0, 50.0}));
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers[1] == Approx(42.2));
        CHECK(mappedNumbers[2] == Approx(63.3));
        CHECK(mappedNumbers[3] == Approx(84.4));
        CHECK(mappedNumbers[4] == Approx(105.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Map Binary Inline", "[MapBinaryInlineTest]")
{
    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };

    SECTION("one number")
    {
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, bf, {11.1}, {10.0});
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers, bf, {11.1, 22.2, 33.3}, {10.0, 20.0, 30.0});
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers[1] == Approx(42.2));
        CHECK(mappedNumbers[2] == Approx(63.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers, bf, {11.1, 22.2, 33.3, 44.4, 55.5}, {10.0, 20.0, 30.0, 40.0, 50.0});
        CHECK(mappedNumbers[0] == Approx(21.1));
        CHECK(mappedNumbers[1] == Approx(42.2));
        CHECK(mappedNumbers[2] == Approx(63.3));
        CHECK(mappedNumbers[3] == Approx(84.4));
        CHECK(mappedNumbers[4] == Approx(105.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Map Ternary Numbers", "[MapTernaryNumbersTest]")
{
    auto tf = [](const TNumber n1, const TNumber n2, const TNumber n3) { return (n1 + n2 + n3); };

    SECTION("one number")
    {
        TNumbers(1) numbers1;
        TNumbers(1) numbers2;
        TNumbers(1) numbers3;
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, tf, Values(numbers1, {11.1}), Values(numbers2, {10.0}), Values(numbers3, {20.0}));
        CHECK(mappedNumbers[0] == Approx(41.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers1;
        TNumbers(3) numbers2;
        TNumbers(3) numbers3;
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers,
            tf,
            Values(numbers1, {11.1, 22.2, 33.3}),
            Values(numbers2, {10.0, 20.0, 30.0}),
            Values(numbers3, {1.0, 2.0, 3.0}));
        CHECK(mappedNumbers[0] == Approx(22.1));
        CHECK(mappedNumbers[1] == Approx(44.2));
        CHECK(mappedNumbers[2] == Approx(66.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers1;
        TNumbers(5) numbers2;
        TNumbers(5) numbers3;
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers,
            tf,
            Values(numbers1, {11.1, 22.2, 33.3, 44.4, 55.5}),
            Values(numbers2, {10.0, 20.0, 30.0, 40.0, 50.0}),
            Values(numbers3, {1.0, 2.0, 3.0, 4.0, 5.0}));
        CHECK(mappedNumbers[0] == Approx(22.1));
        CHECK(mappedNumbers[1] == Approx(44.2));
        CHECK(mappedNumbers[2] == Approx(66.3));
        CHECK(mappedNumbers[3] == Approx(88.4));
        CHECK(mappedNumbers[4] == Approx(110.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Map Ternary Inline", "[MapTernaryInlineTest]")
{
    auto tf = [](const TNumber n1, const TNumber n2, const TNumber n3) { return (n1 + n2 + n3); };

    SECTION("one number")
    {
        TNumbers(1) mappedNumbers;
        Map(mappedNumbers, tf, {11.1}, {10.0}, {1.0});
        CHECK(mappedNumbers[0] == Approx(22.1));
        CHECK(mappedNumbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) mappedNumbers;
        Map(mappedNumbers, tf, {11.1, 22.2, 33.3}, {10.0, 20.0, 30.0}, {1.0, 2.0, 3.0});
        CHECK(mappedNumbers[0] == Approx(22.1));
        CHECK(mappedNumbers[1] == Approx(44.2));
        CHECK(mappedNumbers[2] == Approx(66.3));
        CHECK(mappedNumbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) mappedNumbers;
        Map(mappedNumbers,
            tf,
            {11.1, 22.2, 33.3, 44.4, 55.5},
            {10.0, 20.0, 30.0, 40.0, 50.0},
            {1.0, 2.0, 3.0, 4.0, 5.0});
        CHECK(mappedNumbers[0] == Approx(22.1));
        CHECK(mappedNumbers[1] == Approx(44.2));
        CHECK(mappedNumbers[2] == Approx(66.3));
        CHECK(mappedNumbers[3] == Approx(88.4));
        CHECK(mappedNumbers[4] == Approx(110.5));
        CHECK(mappedNumbers.size() == 5);
    }
}

TEST_CASE("Range", "[RangeTest]")
{
    TNumbers(5) range;
    TNumbers(5) answer;

    CHECK(Range(range) == Values(answer, {0.0, 1.0, 2.0, 3.0, 4.0}));
    CHECK(Range(range, 10) == Values(answer, {10.0, 11.0, 12.0, 13.0, 14.0, 15.0}));
}

TEST_CASE("Reduce Numbers", "[ReduceNumbersTest]")
{
    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };

    SECTION("three numbers")
    {
        TNumbers(3) numbers;
        TNumber answer;
        Reduce(answer, bf, Values(numbers, {11.1, 22.2, 33.3}));
        CHECK(answer == Approx(66.6));
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers;
        TNumber answer;
        Reduce(answer, bf, Values(numbers, {11.1, 22.2, 33.3, 44.4, 55.5}));
        CHECK(answer == Approx(166.5));
    }
}

TEST_CASE("Reduce Inline", "[ReduceInlineTest]")
{
    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };

    SECTION("three numbers")
    {
        TNumber answer;
        Reduce(answer, bf, {11.1, 22.2, 33.3});
        CHECK(answer == Approx(66.6));
    }
    SECTION("five numbers")
    {
        TNumber number;
        Reduce(number, bf, {11.1, 22.2, 33.3, 44.4, 55.5});
        CHECK(number == Approx(166.5));
    }
}

TEST_CASE("Add", "[AddTest]")
{
    SECTION("BinaryAdd")
    {
        CHECK(BinaryAdd(3.0, 5.0) == Approx(8.0));
    }
    SECTION("Add Number Numbers")
    {
        SECTION("one number")
        {
            TNumbers(1) numbers;
            TNumbers(1) answer;
            Add(answer, 1.0, Values(numbers, {2.0}));
            CHECK(answer[0] == Approx(3.0));
            CHECK(answer.size() == 1);
        }
        SECTION("three number")
        {
            TNumbers(3) numbers;
            TNumbers(3) answer;
            Add(answer, 1.0, Values(numbers, {10.0, 20.0, 30.0}));
            CHECK(answer[0] == Approx(11.0));
            CHECK(answer[1] == Approx(21.0));
            CHECK(answer[2] == Approx(31.0));
            CHECK(answer.size() == 3);
        }
    }
    SECTION("Add Numbers")
    {
        SECTION("one number")
        {
            TNumbers(1) numbers;
            TNumbers(1) answer;
            Add(answer, Values(numbers, {1.0}), Values(numbers, {2.0}));
            CHECK(answer[0] == Approx(3.0));
            CHECK(answer.size() == 1);
        }
        SECTION("three number")
        {
            TNumbers(3) numbers;
            TNumbers(3) answer;
            Add(answer, Values(numbers, {1.0, 2.0, 3.0}), Values(numbers, {10.0, 20.0, 30.0}));
            CHECK(answer[0] == Approx(11.0));
            CHECK(answer[1] == Approx(22.0));
            CHECK(answer[2] == Approx(33.0));
            CHECK(answer.size() == 3);
        }
    }
    SECTION("Add Inline")
    {
        SECTION("one number")
        {
            TNumbers(1) answer;
            Add(answer, {1.0}, {2.0});
            CHECK(answer[0] == Approx(3.0));
            CHECK(answer.size() == 1);
        }
        SECTION("three numbers")
        {
            TNumbers(3) answer;
            Add(answer, {1.0, 2.0, 3.0}, {10.0, 20.0, 30.0});
            CHECK(answer[0] == Approx(11.0));
            CHECK(answer[1] == Approx(22.0));
            CHECK(answer[2] == Approx(33.0));
            CHECK(answer.size() == 3);
        }
    }
    SECTION("Add Numbers Reduce")
    {
        SECTION("one number")
        {
            TNumbers(2) numbers;
            TNumber answer;
            Add(answer, Values(numbers, {1.0, 2.0}));
            CHECK(answer == Approx(3.0));
        }
        SECTION("three numbers")
        {
            TNumbers(3) numbers;
            TNumber answer;
            Add(answer, Values(numbers, {1.0, 2.0, 3.0}));
            CHECK(answer == Approx(6.0));
        }
    }
    SECTION("Add Inline Reduce")
    {
        SECTION("one number")
        {
            TNumber answer;
            Add(answer, {1.0, 2.0});
            CHECK(answer == Approx(3.0));
        }
        SECTION("three numbers")
        {
            TNumber answer;
            Add(answer, {1.0, 2.0, 3.0});
            CHECK(answer == Approx(6.0));
        }
    }
}

// Only testing binary functions for Divide, Multiply and Subtract because the macros were tested with Add tests
TEST_CASE("Divide", "[DivideTest]")
{
    SECTION("BinaryDivide")
    {
        CHECK(BinaryDivide(3.0, 5.0) == Approx(0.6));
    }
}

TEST_CASE("Multiply", "[MultiplyTest]")
{
    SECTION("BinaryMultiply")
    {
        CHECK(BinaryMultiply(3.0, 5.0) == Approx(15.0));
    }
}

TEST_CASE("Subtract", "[SubtractTest]")
{
    SECTION("BinarySubtract")
    {
        CHECK(BinarySubtract(3.0, 5.0) == Approx(-2.0));
    }
}

TEST_CASE("Values", "[ValuesTest]")
{
    SECTION("one number")
    {
        TNumbers(1) numbers;
        Values(numbers, {11.1});
        CHECK(numbers[0] == Approx(11.1));
        CHECK(numbers.size() == 1);
    }
    SECTION("three numbers")
    {
        TNumbers(3) numbers;
        Values(numbers, {11.1, 22.2, 33.3});
        CHECK(numbers[0] == Approx(11.1));
        CHECK(numbers[1] == Approx(22.2));
        CHECK(numbers[2] == Approx(33.3));
        CHECK(numbers.size() == 3);
    }
    SECTION("five numbers")
    {
        TNumbers(5) numbers;
        Values(numbers, {11.1, 22.2, 33.3, 44.4, 55.5});
        CHECK(numbers[0] == Approx(11.1));
        CHECK(numbers[1] == Approx(22.2));
        CHECK(numbers[2] == Approx(33.3));
        CHECK(numbers[3] == Approx(44.4));
        CHECK(numbers[4] == Approx(55.5));
        CHECK(numbers.size() == 5);
    }
}
