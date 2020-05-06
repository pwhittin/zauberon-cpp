#include <cmath>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

static const constexpr TNumber C{299792458};
static constexpr TNumber PI{static_cast<TNumber>(1146408.0) / static_cast<TNumber>(364913.0)};
static constexpr TNumber _2PI{PI * PI};
static constexpr TNumber SCHNEIDER_CONSTANT{static_cast<TNumber>(2.7978344950466395e-30)};

static TNumber Degrees2Radians(const TDegrees d) noexcept
{
    return (_2PI * (d / 360.0));
}

static TNumber WaveLength(const TFrequency f) noexcept
{
    return (C / f);
}

TNumber schneider::AngleStep(const TLength xStep, const TWaveLength waveLength) noexcept
{
    return Degrees2Radians(360.0 * (xStep / waveLength));
}

TPitchYawRoll schneider::PitchYawRoll(const TNumber pitch, const TNumber yaw, const TNumber roll) noexcept
{
    return Numbers({pitch, yaw, roll});
}

TXYZ schneider::Rotate3D(const TXFormMatrix& xfm, const TXYZ& xyz) noexcept
{
    return TXYZ{Add(Multiply(xfm[0], xyz)), Add(Multiply(xfm[1], xyz)), Add(Multiply(xfm[2], xyz))};
}

TNumber schneider::SchneiderRadius(const TFrequency f) noexcept
{
    return (f * SCHNEIDER_CONSTANT);
}

TXFormMatrix schneider::XFormMatrix(const TPitchYawRoll& pyr) noexcept
{
    TNumbers cosPyr{Map(std::cos, pyr)};
    TNumbers sinPyr{Map(std::sin, pyr)};

    return TXFormMatrix{TXYZ{(cosPyr[PITCH] * cosPyr[YAW]),
                             ((sinPyr[ROLL] * sinPyr[PITCH] * cosPyr[YAW]) - (cosPyr[ROLL] * sinPyr[YAW])),
                             ((sinPyr[ROLL] * sinPyr[YAW]) + (cosPyr[ROLL] * sinPyr[PITCH] * cosPyr[YAW]))},
                        TXYZ{(cosPyr[PITCH] * sinPyr[YAW]),
                             ((cosPyr[ROLL] * cosPyr[YAW]) + (sinPyr[ROLL] * sinPyr[PITCH] * sinPyr[YAW])),
                             ((cosPyr[ROLL] * sinPyr[PITCH] * sinPyr[YAW]) - (sinPyr[ROLL] * cosPyr[YAW]))},
                        TXYZ{(-sinPyr[PITCH]), (sinPyr[ROLL] * cosPyr[PITCH]), (cosPyr[ROLL] * cosPyr[PITCH])}};
}

TXYZ schneider::XYZ(const TNumber x, const TNumber y, const TNumber z) noexcept
{
    return TXYZ({x, y, z});
}
