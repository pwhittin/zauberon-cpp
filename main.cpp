#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define DEFINE_REDUCER(name, op)                                                                                       \
    NumberType Binary##name(const NumberType n1, const NumberType n2) noexcept                                         \
    {                                                                                                                  \
        return (n1 op n2);                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    NumberType name(const NumbersType& numbers) noexcept                                                               \
    {                                                                                                                  \
        return Reduce(Binary##name, numbers);                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    NumbersType name(const NumbersType& numbers1, const NumbersType& numbers2) noexcept                                \
    {                                                                                                                  \
        return Map(Binary##name, numbers1, numbers2);                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    NumberType name(std::initializer_list<NumberType> numbers) noexcept                                                \
    {                                                                                                                  \
        return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), std::ptr_fun(Binary##name));      \
    }

/***********************************************************************************************************************
  Given
      e = energy
      h = Plank's constant
      c = speed of light
      m = mass
      f = Zauberon circular helical frequency
      r = Zauberon circular helical radius
      z = Zauberon mass
  then
      e = (h * f)
      e = (m * (c * c))
      z = ((pi * r * r) * (c / f))  // volume of one Zauberon wave length
  thus
      (((pi * r * r) * (c / f)) * (c * c)) = (h * f)
      ((pi * r * r * c * c * c) / f)       = (h * f)
      (r * r)                              = ((h * f * f) / (pi * c * c * c))
      r                                    = (sqrt(h / (pi * c * c * c)) * f)
      SchneiderConstant                    = sqrt(h / (pi * c * c * c))
  hence
      schneiderRadius = (schneiderConstant * f)
***********************************************************************************************************************/

using NumberType = double;

using DegreesType = NumberType;
using FrequencyType = NumberType;
using LengthType = NumberType;
using PitchType = NumberType;
using RadiansType = NumberType;
using RadiusType = NumberType;
using RollType = NumberType;
using WaveLengthType = NumberType;
using YawType = NumberType;

using NumbersType = std::vector<NumberType>;
using PitchYawRollType = NumbersType;
using XFormMatrixType = std::vector<NumbersType>;
using XYZType = NumbersType;

using UnaryFunctionType = NumberType (*)(const NumberType);
using BinaryFunctionType = NumberType (*)(const NumberType, const NumberType);

enum PitchYarRollEnum
{
    PITCHI,
    YAWI,
    ROLLI
};

const NumberType PI{std::acos(-1)};
const NumberType _2PI{PI * PI};

const NumberType H{6.62607004E-34};
const NumberType C{299792458};
const NumberType SCHNEIDER_CONSTANT{std::sqrt(H / (PI * C * C * C))};

// Given a frequency 'f', return the Schneider circular helical radius
auto F2SchneiderRadius(const FrequencyType f) noexcept
{
    return (f * SCHNEIDER_CONSTANT);
}

auto F2WaveLength(const FrequencyType f) noexcept
{
    return (C / f);
}

auto Degrees2Radians(const DegreesType d) noexcept
{
    return (_2PI * (d / 360.0));
}

auto WaveLength2AngleStep(const LengthType xStep, const WaveLengthType waveLength) noexcept
{
    return Degrees2Radians(360.0 * (xStep / waveLength));
}

NumbersType Numbers(std::initializer_list<NumberType> numbers) noexcept
{
    NumbersType result;
    result.insert(result.end(), numbers.begin(), numbers.end());
    return result;
}

XYZType XYZ(const NumberType x = 0.0, const NumberType y = 0.0, const NumberType z = 0.0) noexcept
{
    return Numbers({x, y, z});
}

XFormMatrixType
XFormMatrix(const XYZType& row0 = XYZ(), const XYZType& row1 = XYZ(), const XYZType& row2 = XYZ()) noexcept
{
    return XFormMatrixType{XYZType(row0), XYZType(row1), XYZType(row2)};
}

NumbersType Map(UnaryFunctionType uf, const NumbersType& numbers) noexcept
{
    auto result{NumbersType(numbers.size())};
    std::transform(numbers.cbegin(), numbers.cend(), result.begin(), uf);
    return result;
}

NumbersType Map(BinaryFunctionType bf, const NumbersType& numbers1, const NumbersType& numbers2) noexcept
{
    auto result{NumbersType(numbers1.size())};
    std::transform(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), result.begin(), bf);
    return result;
}

NumbersType Map(UnaryFunctionType uf, std::initializer_list<NumberType> numbers) noexcept
{
    auto result{NumbersType(numbers.size())};
    std::transform(numbers.begin(), numbers.end(), result.begin(), uf);
    return result;
}

NumbersType Map(BinaryFunctionType bf,
                std::initializer_list<NumberType> numbers1,
                std::initializer_list<NumberType> numbers2) noexcept
{
    auto result{NumbersType(std::max(numbers1.size(), numbers1.size()))};
    std::transform(numbers1.begin(), numbers1.end(), numbers2.begin(), result.begin(), bf);
    return result;
}

NumberType Reduce(BinaryFunctionType bf, const NumbersType& numbers) noexcept
{
    return std::accumulate(numbers.cbegin() + 1, numbers.cend(), *numbers.cbegin(), bf);
}

NumberType Reduce(BinaryFunctionType bf, std::initializer_list<NumberType> numbers) noexcept
{
    return std::accumulate(numbers.begin() + 1, numbers.end(), *numbers.begin(), bf);
}

DEFINE_REDUCER(Add, +)
DEFINE_REDUCER(Divide, /)
DEFINE_REDUCER(Multiply, *)
DEFINE_REDUCER(Subtract, -)

auto PitchYawRoll2XformMatrix(const PitchYawRollType& pyr) noexcept
{
    NumbersType cosPyr{Map(std::cos, pyr)};
    NumbersType sinPyr{Map(std::sin, pyr)};
    XFormMatrixType result(XFormMatrix());

    result[0][0] = (cosPyr[PITCHI] * cosPyr[YAWI]);
    result[0][1] = (sinPyr[ROLLI] * sinPyr[PITCHI] * cosPyr[YAWI]) - (cosPyr[ROLLI] * sinPyr[YAWI]);
    result[0][2] = (sinPyr[ROLLI] * sinPyr[YAWI]) + (cosPyr[ROLLI] * sinPyr[PITCHI] * cosPyr[YAWI]);

    result[1][0] = (cosPyr[PITCHI] * sinPyr[YAWI]);
    result[1][1] = (cosPyr[ROLLI] * cosPyr[YAWI]) + (sinPyr[ROLLI] * sinPyr[PITCHI] * sinPyr[YAWI]);
    result[1][2] = (cosPyr[ROLLI] * sinPyr[PITCHI] * sinPyr[YAWI]) - (sinPyr[ROLLI] * cosPyr[YAWI]);

    result[2][0] = (-sinPyr[PITCHI]);
    result[2][1] = (sinPyr[ROLLI] * cosPyr[PITCHI]);
    result[2][2] = (cosPyr[ROLLI] * cosPyr[PITCHI]);

    return result;
}

auto Rotate3D(const XFormMatrixType& xfm, const XYZType& xyz) noexcept
{
    return XYZType{Add(Multiply(xfm[0], xyz)), Add(Multiply(xfm[1], xyz)), Add(Multiply(xfm[2], xyz))};
}

auto FormatNumberVector(const NumbersType& nv) noexcept
{
    auto result{std::string("[")};
    for (auto i : nv)
        result += std::to_string(i) + " ";
    result.back() = ']';
    return result;
}

auto FormatXYZ(const XYZType& xyz) noexcept
{
    return std::string{"[" + std::to_string(xyz[0]) + " " + std::to_string(xyz[1]) + " " + std::to_string(xyz[2]) +
                       "]"};
}

auto FormatXformMatrix(const XFormMatrixType& xfm) noexcept
{
    return std::string{"[" + FormatXYZ(xfm[0]) + "\n " + FormatXYZ(xfm[1]) + "\n " + FormatXYZ(xfm[2]) + "]"};
}

NumberType TimesTwo(const NumberType n) noexcept
{
    return 2 * n;
}

int main()
{
    PitchYawRollType pyr{XYZ(1, 2, 3)};
    auto xfm{PitchYawRoll2XformMatrix(pyr)};
    XYZType xyz{XYZ(11, 22, 33)};
    auto xyzRotated{Rotate3D(xfm, xyz)};
    std::cout << "\n";
    std::cout << FormatNumberVector(pyr) << "\n";
    std::cout << "\n";
    std::cout << FormatXformMatrix(xfm) << "\n";
    std::cout << "\n";
    std::cout << FormatXYZ(xyz) << "\n";
    std::cout << "\n";
    std::cout << FormatXYZ(xyzRotated) << "\n";
    return 0;
}
