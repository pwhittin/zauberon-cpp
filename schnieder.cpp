#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

static const constexpr TNumber C{299792458};
static constexpr TNumber PI{static_cast<TNumber>(1146408.0) / static_cast<TNumber>(364913.0)};
static constexpr TNumber _2PI{PI * PI};
static constexpr TNumber SCHNEIDER_CONSTANT{static_cast<TNumber>(2.7978344950466395e-30)};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TNumber WaveLength(TFrequency f) noexcept;

static TNumbers Frequencies{Multiply(1e+12, Range(405, 791))};
static TNumbers WaveLengths{Map(WaveLength, Frequencies)};
static TNumbers Radii{Map(SchneiderRadius, Frequencies)};

static TNumber Degrees2Radians(const TDegrees d) noexcept
{
    return (_2PI * (d / 360.0));
}

static TLength Radius(const TWaveLength waveLength) noexcept
{
    auto waveLengthIt{std::find(WaveLengths.begin(), WaveLengths.end(), waveLength)};
    auto waveLengthIndex{std::distance(WaveLengths.begin(), waveLengthIt)};
    return Radii[waveLengthIndex];
}

static TRadians RandomAngle() noexcept
{
    return Degrees2Radians(static_cast<TDegrees>(std::rand() % 360));
}

static TRotationEnum RandomRotation() noexcept
{
    return ((std::rand() % 2) == 0) ? R_RIGHT : R_LEFT;
}

static TPitchYawRoll RandomPitchYawRoll() noexcept
{
    return PitchYawRoll(RandomAngle(), RandomAngle(), RandomAngle());
}

static TWaveLength RandomWaveLength() noexcept
{
    return WaveLengths[std::rand() % WaveLengths.size()];
}

static TNumber WaveLength(const TFrequency f) noexcept
{
    return (C / f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// external
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TRadians schneider::AngleStep(const TLength xStep, const TWaveLength waveLength) noexcept
{
    return Degrees2Radians(360.0 * (xStep / waveLength));
}

// TODO: Add tests
TZauberon schneider::InitializeZauberon(const TLength xStep, const TXYZ& xyzInitial) noexcept
{
    TWaveLength waveLength{RandomWaveLength()};

    TZauberon result;
    result.angle = RandomAngle();
    result.angleStep = AngleStep(xStep, waveLength);
    result.pyr = RandomPitchYawRoll();
    result.radius = Radius(waveLength);
    result.rotation = RandomRotation();
    result.xfm = XFormMatrix(result.pyr);
    result.xyz = xyzInitial;
    return result;
}

TPitchYawRoll schneider::PitchYawRoll(const TPitch pitch, const TYaw yaw, const TRoll roll) noexcept
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

    return TXFormMatrix{
        TXYZ{(cosPyr[PYR_PITCH] * cosPyr[PYR_YAW]),
             ((sinPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * cosPyr[PYR_YAW]) - (cosPyr[PYR_ROLL] * sinPyr[PYR_YAW])),
             ((sinPyr[PYR_ROLL] * sinPyr[PYR_YAW]) + (cosPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * cosPyr[PYR_YAW]))},
        TXYZ{(cosPyr[PYR_PITCH] * sinPyr[PYR_YAW]),
             ((cosPyr[PYR_ROLL] * cosPyr[PYR_YAW]) + (sinPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * sinPyr[PYR_YAW])),
             ((cosPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * sinPyr[PYR_YAW]) - (sinPyr[PYR_ROLL] * cosPyr[PYR_YAW]))},
        TXYZ{(-sinPyr[PYR_PITCH]), (sinPyr[PYR_ROLL] * cosPyr[PYR_PITCH]), (cosPyr[PYR_ROLL] * cosPyr[PYR_PITCH])}};
}

TXYZ schneider::XYZ(const TX x, const TY y, const TZ z) noexcept
{
    return TXYZ({x, y, z});
}
