#include <array>
#include "../functional-utils.h"
#include "catch.hpp"

using namespace functional_utils;

// TODO: Fix all unit tests.  Consider moving the destination to the first parameter of all array functions
//TEST_CASE("IndexOf", "[IndexOfTest]")
//{
//    SECTION("one number")
//    {
//        TNumbers numbers{Numbers({11.1})};
//        CHECK(IndexOf(numbers, 11.1) == 0);
//        CHECK(IndexOf(numbers, 11.2) == -1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers numbers{Numbers({11.1, 22.2, 33.3})};
//        CHECK(IndexOf(numbers, 11.1) == 0);
//        CHECK(IndexOf(numbers, 22.2) == 1);
//        CHECK(IndexOf(numbers, 33.3) == 2);
//        CHECK(IndexOf(numbers, 44.4) == -1);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers numbers{Numbers({11.1, 22.2, 33.3, 44.4, 55.5})};
//        CHECK(IndexOf(numbers, 11.1) == 0);
//        CHECK(IndexOf(numbers, 22.2) == 1);
//        CHECK(IndexOf(numbers, 33.3) == 2);
//        CHECK(IndexOf(numbers, 44.4) == 3);
//        CHECK(IndexOf(numbers, 55.5) == 4);
//        CHECK(IndexOf(numbers, 66.6) == -1);
//    }
//}
//
//TEST_CASE("Map Unary Numbers", "[MapUnaryNumbersTest]")
//{
//    auto uf = [](const TNumber n) { return (1 + n); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(uf, Numbers({11.1}))};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{Map(uf, Numbers({11.1, 22.2, 33.3}))};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers[1] == Approx(23.2));
//        CHECK(mappedNumbers[2] == Approx(34.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{Map(uf, Numbers({11.1, 22.2, 33.3, 44.4, 55.5}))};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers[1] == Approx(23.2));
//        CHECK(mappedNumbers[2] == Approx(34.3));
//        CHECK(mappedNumbers[3] == Approx(45.4));
//        CHECK(mappedNumbers[4] == Approx(56.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Map Unary Inline", "[MapUnaryInlineTest]")
//{
//    auto uf = [](const TNumber n) { return (1 + n); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(uf, {11.1})};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{Map(uf, {11.1, 22.2, 33.3})};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers[1] == Approx(23.2));
//        CHECK(mappedNumbers[2] == Approx(34.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{Map(uf, {11.1, 22.2, 33.3, 44.4, 55.5})};
//        CHECK(mappedNumbers[0] == Approx(12.1));
//        CHECK(mappedNumbers[1] == Approx(23.2));
//        CHECK(mappedNumbers[2] == Approx(34.3));
//        CHECK(mappedNumbers[3] == Approx(45.4));
//        CHECK(mappedNumbers[4] == Approx(56.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Map Binary Numbers", "[MapBinaryNumbersTest]")
//{
//    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(bf, Numbers({11.1}), Numbers({10.0}))};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{Map(bf, Numbers({11.1, 22.2, 33.3}), Numbers({10.0, 20.0, 30.0}))};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers[1] == Approx(42.2));
//        CHECK(mappedNumbers[2] == Approx(63.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{
//            Map(bf, Numbers({11.1, 22.2, 33.3, 44.4, 55.5}), Numbers({10.0, 20.0, 30.0, 40.0, 50.0}))};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers[1] == Approx(42.2));
//        CHECK(mappedNumbers[2] == Approx(63.3));
//        CHECK(mappedNumbers[3] == Approx(84.4));
//        CHECK(mappedNumbers[4] == Approx(105.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Map Binary Inline", "[MapBinaryInlineTest]")
//{
//    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(bf, {11.1}, {10.0})};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{Map(bf, {11.1, 22.2, 33.3}, {10.0, 20.0, 30.0})};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers[1] == Approx(42.2));
//        CHECK(mappedNumbers[2] == Approx(63.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{Map(bf, {11.1, 22.2, 33.3, 44.4, 55.5}, {10.0, 20.0, 30.0, 40.0, 50.0})};
//        CHECK(mappedNumbers[0] == Approx(21.1));
//        CHECK(mappedNumbers[1] == Approx(42.2));
//        CHECK(mappedNumbers[2] == Approx(63.3));
//        CHECK(mappedNumbers[3] == Approx(84.4));
//        CHECK(mappedNumbers[4] == Approx(105.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Map Ternary Numbers", "[MapTernaryNumbersTest]")
//{
//    auto tf = [](const TNumber n1, const TNumber n2, const TNumber n3) { return (n1 + n2 + n3); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(tf, Numbers({11.1}), Numbers({10.0}), Numbers({20.0}))};
//        CHECK(mappedNumbers[0] == Approx(41.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{
//            Map(tf, Numbers({11.1, 22.2, 33.3}), Numbers({10.0, 20.0, 30.0}), Numbers({1.0, 2.0, 3.0}))};
//        CHECK(mappedNumbers[0] == Approx(22.1));
//        CHECK(mappedNumbers[1] == Approx(44.2));
//        CHECK(mappedNumbers[2] == Approx(66.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{Map(tf,
//                                   Numbers({11.1, 22.2, 33.3, 44.4, 55.5}),
//                                   Numbers({10.0, 20.0, 30.0, 40.0, 50.0}),
//                                   Numbers({1.0, 2.0, 3.0, 4.0, 5.0}))};
//        CHECK(mappedNumbers[0] == Approx(22.1));
//        CHECK(mappedNumbers[1] == Approx(44.2));
//        CHECK(mappedNumbers[2] == Approx(66.3));
//        CHECK(mappedNumbers[3] == Approx(88.4));
//        CHECK(mappedNumbers[4] == Approx(110.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Map Ternary Inline", "[MapTernaryInlineTest]")
//{
//    auto tf = [](const TNumber n1, const TNumber n2, const TNumber n3) { return (n1 + n2 + n3); };
//
//    SECTION("one number")
//    {
//        TNumbers mappedNumbers{Map(tf, {11.1}, {10.0}, {1.0})};
//        CHECK(mappedNumbers[0] == Approx(22.1));
//        CHECK(mappedNumbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers mappedNumbers{Map(tf, {11.1, 22.2, 33.3}, {10.0, 20.0, 30.0}, {1.0, 2.0, 3.0})};
//        CHECK(mappedNumbers[0] == Approx(22.1));
//        CHECK(mappedNumbers[1] == Approx(44.2));
//        CHECK(mappedNumbers[2] == Approx(66.3));
//        CHECK(mappedNumbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers mappedNumbers{
//            Map(tf, {11.1, 22.2, 33.3, 44.4, 55.5}, {10.0, 20.0, 30.0, 40.0, 50.0}, {1.0, 2.0, 3.0, 4.0, 5.0})};
//        CHECK(mappedNumbers[0] == Approx(22.1));
//        CHECK(mappedNumbers[1] == Approx(44.2));
//        CHECK(mappedNumbers[2] == Approx(66.3));
//        CHECK(mappedNumbers[3] == Approx(88.4));
//        CHECK(mappedNumbers[4] == Approx(110.5));
//        CHECK(mappedNumbers.size() == 5);
//    }
//}
//
//TEST_CASE("Numbers", "[NumbersTest]")
//{
//    SECTION("one number")
//    {
//        TNumbers numbers{Numbers({11.1})};
//        CHECK(numbers[0] == Approx(11.1));
//        CHECK(numbers.size() == 1);
//    }
//    SECTION("three numbers")
//    {
//        TNumbers numbers{Numbers({11.1, 22.2, 33.3})};
//        CHECK(numbers[0] == Approx(11.1));
//        CHECK(numbers[1] == Approx(22.2));
//        CHECK(numbers[2] == Approx(33.3));
//        CHECK(numbers.size() == 3);
//    }
//    SECTION("five numbers")
//    {
//        TNumbers numbers{Numbers({11.1, 22.2, 33.3, 44.4, 55.5})};
//        CHECK(numbers[0] == Approx(11.1));
//        CHECK(numbers[1] == Approx(22.2));
//        CHECK(numbers[2] == Approx(33.3));
//        CHECK(numbers[3] == Approx(44.4));
//        CHECK(numbers[4] == Approx(55.5));
//        CHECK(numbers.size() == 5);
//    }
//}
//
//TEST_CASE("Range", "[RangeTest]")
//{
//    CHECK(Range(5) == Numbers({0.0, 1.0, 2.0, 3.0, 4.0}));
//    CHECK(Range(10, 16) == Numbers({10.0, 11.0, 12.0, 13.0, 14.0, 15.0}));
//}
//
//TEST_CASE("Reduce Numbers", "[ReduceNumbersTest]")
//{
//    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };
//
//    SECTION("three numbers")
//    {
//        TNumber number{Reduce(bf, Numbers({11.1, 22.2, 33.3}))};
//        CHECK(number == Approx(66.6));
//    }
//    SECTION("five numbers")
//    {
//        TNumber number{Reduce(bf, Numbers({11.1, 22.2, 33.3, 44.4, 55.5}))};
//        CHECK(number == Approx(166.5));
//    }
//}
//
//TEST_CASE("Reduce Inline", "[ReduceInlineTest]")
//{
//    auto bf = [](const TNumber n1, const TNumber n2) { return (n1 + n2); };
//
//    SECTION("three numbers")
//    {
//        TNumber number{Reduce(bf, {11.1, 22.2, 33.3})};
//        CHECK(number == Approx(66.6));
//    }
//    SECTION("five numbers")
//    {
//        TNumber number{Reduce(bf, {11.1, 22.2, 33.3, 44.4, 55.5})};
//        CHECK(number == Approx(166.5));
//    }
//}
//
//TEST_CASE("Add", "[AddTest]")
//{
//    SECTION("BinaryAdd")
//    {
//        CHECK(BinaryAdd(3.0, 5.0) == Approx(8.0));
//    }
//    SECTION("Add Number Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Add(1.0, Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(3.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Add(1.0, Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(11.0));
//            CHECK(numbers[1] == Approx(21.0));
//            CHECK(numbers[2] == Approx(31.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Add Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Add(Numbers({1.0}), Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(3.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Add(Numbers({1.0, 2.0, 3.0}), Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(11.0));
//            CHECK(numbers[1] == Approx(22.0));
//            CHECK(numbers[2] == Approx(33.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Add Inline")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Add({1.0}, {2.0})};
//            CHECK(numbers[0] == Approx(3.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three numbers")
//        {
//            TNumbers numbers{Add({1.0, 2.0, 3.0}, {10.0, 20.0, 30.0})};
//            CHECK(numbers[0] == Approx(11.0));
//            CHECK(numbers[1] == Approx(22.0));
//            CHECK(numbers[2] == Approx(33.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Add Numbers Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Add(Numbers({1.0, 2.0}))};
//            CHECK(number == Approx(3.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Add(Numbers({1.0, 2.0, 3.0}))};
//            CHECK(number == Approx(6.0));
//        }
//    }
//    SECTION("Add Inline Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Add({1.0, 2.0})};
//            CHECK(number == Approx(3.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Add({1.0, 2.0, 3.0})};
//            CHECK(number == Approx(6.0));
//        }
//    }
//}
//
//TEST_CASE("Divide", "[DivideTest]")
//{
//    SECTION("BinaryDivide")
//    {
//        CHECK(BinaryDivide(3.0, 5.0) == Approx(0.6));
//    }
//    SECTION("Divide Number Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Divide(2.0, Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(1.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Divide(2.0, Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(5.0));
//            CHECK(numbers[1] == Approx(10.0));
//            CHECK(numbers[2] == Approx(15.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Divide Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Divide(Numbers({1.0}), Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(0.5));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Divide(Numbers({1.0, 2.0, 3.0}), Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(0.1));
//            CHECK(numbers[1] == Approx(0.1));
//            CHECK(numbers[2] == Approx(0.1));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Divide Inline")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Divide({1.0}, {2.0})};
//            CHECK(numbers[0] == Approx(0.5));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three numbers")
//        {
//            TNumbers numbers{Divide({1.0, 2.0, 3.0}, {10.0, 20.0, 30.0})};
//            CHECK(numbers[0] == Approx(0.1));
//            CHECK(numbers[1] == Approx(0.1));
//            CHECK(numbers[2] == Approx(0.1));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Divide Numbers Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Divide(Numbers({1.0, 2.0}))};
//            CHECK(number == Approx(0.5));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Divide(Numbers({1.0, 2.0, 3.0}))};
//            CHECK(number == Approx(0.166666667));
//        }
//    }
//    SECTION("Divide Inline Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Divide({1.0, 2.0})};
//            CHECK(number == Approx(0.5));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Divide({1.0, 2.0, 3.0})};
//            CHECK(number == Approx(0.166666667));
//        }
//    }
//}
//
//TEST_CASE("Multiply", "[MultiplyTest]")
//{
//    SECTION("BinaryMultiply")
//    {
//        CHECK(BinaryMultiply(3.0, 5.0) == Approx(15.0));
//    }
//    SECTION("Multiply Number Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Multiply(2.0, Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(4.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Multiply(2.0, Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(20.0));
//            CHECK(numbers[1] == Approx(40.0));
//            CHECK(numbers[2] == Approx(60.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Multiply Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Multiply(Numbers({1.0}), Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(2.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Multiply(Numbers({1.0, 2.0, 3.0}), Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(10.0));
//            CHECK(numbers[1] == Approx(40.0));
//            CHECK(numbers[2] == Approx(90.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Multiply Inline")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Multiply({1.0}, {2.0})};
//            CHECK(numbers[0] == Approx(2.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three numbers")
//        {
//            TNumbers numbers{Multiply({1.0, 2.0, 3.0}, {10.0, 20.0, 30.0})};
//            CHECK(numbers[0] == Approx(10.0));
//            CHECK(numbers[1] == Approx(40.0));
//            CHECK(numbers[2] == Approx(90.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Multiply Numbers Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Multiply(Numbers({1.0, 2.0}))};
//            CHECK(number == Approx(2.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Multiply(Numbers({1.0, 2.0, 3.0}))};
//            CHECK(number == Approx(6.0));
//        }
//    }
//    SECTION("Multiply Inline Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Multiply({1.0, 2.0})};
//            CHECK(number == Approx(2.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Multiply({1.0, 2.0, 3.0})};
//            CHECK(number == Approx(6.0));
//        }
//    }
//}
//
//TEST_CASE("Subtract", "[SubtractTest]")
//{
//    SECTION("BinarySubtract")
//    {
//        CHECK(BinarySubtract(3.0, 5.0) == Approx(-2.0));
//    }
//    SECTION("Subtract Number Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Subtract(2.0, Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(0.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Subtract(2.0, Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(8.0));
//            CHECK(numbers[1] == Approx(18.0));
//            CHECK(numbers[2] == Approx(28.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Subtract Numbers")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Subtract(Numbers({1.0}), Numbers({2.0}))};
//            CHECK(numbers[0] == Approx(-1.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three number")
//        {
//            TNumbers numbers{Subtract(Numbers({1.0, 2.0, 3.0}), Numbers({10.0, 20.0, 30.0}))};
//            CHECK(numbers[0] == Approx(-9.0));
//            CHECK(numbers[1] == Approx(-18.0));
//            CHECK(numbers[2] == Approx(-27.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Subtract Inline")
//    {
//        SECTION("one number")
//        {
//            TNumbers numbers{Subtract({1.0}, {2.0})};
//            CHECK(numbers[0] == Approx(-1.0));
//            CHECK(numbers.size() == 1);
//        }
//        SECTION("three numbers")
//        {
//            TNumbers numbers{Subtract({1.0, 2.0, 3.0}, {10.0, 20.0, 30.0})};
//            CHECK(numbers[0] == Approx(-9.0));
//            CHECK(numbers[1] == Approx(-18.0));
//            CHECK(numbers[2] == Approx(-27.0));
//            CHECK(numbers.size() == 3);
//        }
//    }
//    SECTION("Subtract Numbers Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Subtract(Numbers({1.0, 2.0}))};
//            CHECK(number == Approx(-1.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Subtract(Numbers({1.0, 2.0, 3.0}))};
//            CHECK(number == Approx(-4.0));
//        }
//    }
//    SECTION("Subtract Inline Reduce")
//    {
//        SECTION("one number")
//        {
//            TNumber number{Subtract({1.0, 2.0})};
//            CHECK(number == Approx(-1.0));
//        }
//        SECTION("three numbers")
//        {
//            TNumber number{Subtract({1.0, 2.0, 3.0})};
//            CHECK(number == Approx(-4.0));
//        }
//    }
//}
