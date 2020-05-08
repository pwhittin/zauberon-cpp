#include <cmath>
#include <cstdlib>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

static const TNumber C{299792458};
static const TNumber H{6.62607004e-34};
static constexpr TNumber PI{static_cast<TNumber>(1146408.0) / static_cast<TNumber>(364913.0)};
static constexpr TNumber PI_2{PI * PI};

static const TNumber BOX{100000.0};
static const TNumber SPACE{BOX / 2.0};
static const TNumber CENTER_X{BOX / 2.0};
static const TNumber CENTER_Y{BOX / 2.0};
static const TNumber CENTER_Z{BOX / 2.0};
static const TNumber BACK_BOUNDARY{CENTER_X + (SPACE / 2.0)};
static const TNumber FRONT_BOUNDARY{CENTER_X - (SPACE / 2.0)};
static const TNumber RIGHT_BOUNDARY{CENTER_Y + (SPACE / 2.0)};
static const TNumber LEFT_BOUNDARY{CENTER_Y - (SPACE / 2.0)};
static const TNumber UP_BOUNDARY{CENTER_Z - (SPACE / 2.0)};
static const TNumber DOWN_BOUNDARY{CENTER_Z + (SPACE / 2.0)};
static const TNumber NEGATIVE_SPACE{-SPACE};
static const TNumber POSITIVE_SPACE{SPACE};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TNumber WaveLength(TFrequency f) noexcept;
static int Random() noexcept;

static TNumbers Frequencies{Multiply(1e+12, Range(405, 791))};
static TNumbers WaveLengths{Map(WaveLength, Frequencies)};
static TNumbers Radii{Map(SchneiderRadius, Frequencies)};

static TNumber Degrees2Radians(const TDegrees d) noexcept
{
    return (PI_2 * (d / 360.0));
}

static TLength Radius(const TWaveLength waveLength) noexcept
{
    return Radii[IndexOf(WaveLengths, waveLength)];
}

static int Random() noexcept
{
    return std::rand();
}

static TRadians RandomAngle() noexcept
{
    return Degrees2Radians(static_cast<TDegrees>(Random() % 360));
}

static TRotationEnum RandomRotation() noexcept
{
    return ((Random() % 2) == 0) ? R_RIGHT : R_LEFT;
}

static TPitchYawRoll RandomPitchYawRoll() noexcept
{
    return PitchYawRoll(RandomAngle(), RandomAngle(), RandomAngle());
}

static TWaveLength RandomWaveLength() noexcept
{
    return WaveLengths[Random() % WaveLengths.size()];
}

TNumber SpaceAdjust(const TNumber xYorZ, const TNumber positiveBoundary, const TNumber negativeBoundary) noexcept
{
    return (xYorZ < positiveBoundary) ? POSITIVE_SPACE : ((xYorZ > negativeBoundary) ? NEGATIVE_SPACE : xYorZ);
}

TXYZ XYZSpaceAdjustments(const TXYZ& xyz) noexcept
{
    return Map(SpaceAdjust,
               xyz,
               XYZ(FRONT_BOUNDARY, LEFT_BOUNDARY, UP_BOUNDARY),
               XYZ(BACK_BOUNDARY, RIGHT_BOUNDARY, DOWN_BOUNDARY));
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
    return Degrees2Radians(std::fmod((360.0 * (xStep / waveLength)), 360.0));
}

TPitchYawRoll schneider::PitchYawRoll(const TPitch pitch, const TYaw yaw, const TRoll roll) noexcept
{
    return Numbers({pitch, yaw, roll});
}

TXYZ schneider::Rotate3D(const TXFormMatrix& xfm, const TXYZ& xyz) noexcept
{
    return TXYZ{Add(Multiply(xfm[0], xyz)), Add(Multiply(xfm[1], xyz)), Add(Multiply(xfm[2], xyz))};
}

static TNumber SCHNEIDER_CONSTANT{std::sqrt(H / (PI * C * C * C))};
TNumber schneider::SchneiderRadius(const TFrequency f) noexcept
{
    return (f * SCHNEIDER_CONSTANT);
}

TXFormMatrix schneider::XFormMatrix(const TPitchYawRoll& pyr) noexcept
{
    TNumbers cosPyr{Map(UNARY_FUNCTION(std::cos), pyr)};
    TNumbers sinPyr{Map(UNARY_FUNCTION(std::sin), pyr)};

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

TZauberon schneider::ZauberonInitialize(const TLength xStep, const TXYZ& xyzInitial) noexcept
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

void schneider::ZauberonNewPosition(const TLength xStep, TZauberon& z) noexcept
{
    auto angleNew{(z.rotation == R_RIGHT) ? (z.angle + z.angleStep) : (z.angle - z.angleStep)};
    auto xyzOriginAlongXAxis{XYZ(xStep, (z.radius * std::cos(angleNew)), (z.radius * std::sin(angleNew)))};
    auto xyzOriginAlongHelixAxis{Rotate3D(z.xfm, xyzOriginAlongXAxis)};
    auto xyzNew{Map(UNARY_FUNCTION(std::trunc), Add(z.xyz, xyzOriginAlongHelixAxis))};
    z.angle = angleNew;
    z.xyz = XYZSpaceAdjustments(xyzNew);
}

TZauberons schneider::Zauberons(const TIndex numberOfZauberons) noexcept
{
    return TZauberons{numberOfZauberons};
}
