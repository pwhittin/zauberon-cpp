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

static const int FREQ_START{405};
static const int FREQ_END{791};
static const int FREQ_COUNT{FREQ_END - FREQ_START};

TNumbers(FREQ_COUNT) Frequencies;
TNumbers(FREQ_COUNT) WaveLengths;
TNumbers(FREQ_COUNT) Radii;

static bool Initialize() noexcept
{
    Range(FREQ_START, Frequencies);
    Multiply(1e+12, Frequencies, Frequencies);

    Map(WaveLength, Frequencies, WaveLengths);
    Map(SchneiderRadius, Frequencies, Radii);

    return true;
}
static bool Initialized{Initialize()};

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

static void RandomPitchYawRoll(TPitchYawRoll& pyr) noexcept
{
    PitchYawRoll(RandomAngle(), RandomAngle(), RandomAngle(), pyr);
}

static TWaveLength RandomWaveLength() noexcept
{
    return WaveLengths[Random() % WaveLengths.size()];
}

TNumber SpaceAdjust(const TNumber xYorZ, const TNumber positiveBoundary, const TNumber negativeBoundary) noexcept
{
    return (xYorZ < positiveBoundary) ? POSITIVE_SPACE : ((xYorZ > negativeBoundary) ? NEGATIVE_SPACE : xYorZ);
}

static TXYZ FRONT_LEFT_UP_BOUNDARIES{FRONT_BOUNDARY, LEFT_BOUNDARY, UP_BOUNDARY};
static TXYZ BACK_RIGHT_DOWN_BOUNDARIES{BACK_BOUNDARY, RIGHT_BOUNDARY, DOWN_BOUNDARY};
void XYZSpaceAdjustments(TXYZ& xyz) noexcept
{
    Map(SpaceAdjust, xyz, FRONT_LEFT_UP_BOUNDARIES, BACK_RIGHT_DOWN_BOUNDARIES, xyz);
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

void schneider::PitchYawRoll(const TPitch pitch, const TYaw yaw, const TRoll roll, TPitchYawRoll& pyr) noexcept
{
    pyr[PYR_PITCH] = pitch;
    pyr[PYR_YAW] = yaw;
    pyr[PYR_ROLL] = roll;
}

void schneider::Rotate3D(const TXFormMatrix& xfm, const TXYZ& xyz, TXYZ& xyzRotated) noexcept
{
    TXYZ xyzTemp;

    Multiply(xfm[XYZ_X], xyz, xyzTemp);
    Add(xyzTemp, xyzRotated[XYZ_X]);

    Multiply(xfm[XYZ_Y], xyz, xyzTemp);
    Add(xyzTemp, xyzRotated[XYZ_Y]);

    Multiply(xfm[XYZ_Z], xyz, xyzTemp);
    Add(xyzTemp, xyzRotated[XYZ_Z]);
}

static TNumber SCHNEIDER_CONSTANT{std::sqrt(H / (PI * C * C * C))};
TNumber schneider::SchneiderRadius(const TFrequency f) noexcept
{
    return (f * SCHNEIDER_CONSTANT);
}

void schneider::XFormMatrix(const TPitchYawRoll& pyr, TXFormMatrix& xfm) noexcept
{
    TXYZ cosPyr;
    Map([] UNARY_FN(std::cos), pyr, cosPyr);

    TXYZ sinPyr;
    Map([] UNARY_FN(std::sin), pyr, sinPyr);

    xfm[XYZ_X][XYZ_X] = (cosPyr[PYR_PITCH] * cosPyr[PYR_YAW]);
    xfm[XYZ_X][XYZ_Y] =
        ((sinPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * cosPyr[PYR_YAW]) - (cosPyr[PYR_ROLL] * sinPyr[PYR_YAW]));
    xfm[XYZ_X][XYZ_Z] =
        ((sinPyr[PYR_ROLL] * sinPyr[PYR_YAW]) + (cosPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * cosPyr[PYR_YAW]));

    xfm[XYZ_Y][XYZ_X] = (cosPyr[PYR_PITCH] * sinPyr[PYR_YAW]);
    xfm[XYZ_Y][XYZ_Y] =
        ((cosPyr[PYR_ROLL] * cosPyr[PYR_YAW]) + (sinPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * sinPyr[PYR_YAW]));
    xfm[XYZ_Y][XYZ_Z] =
        ((cosPyr[PYR_ROLL] * sinPyr[PYR_PITCH] * sinPyr[PYR_YAW]) - (sinPyr[PYR_ROLL] * cosPyr[PYR_YAW]));

    xfm[XYZ_Z][XYZ_X] = (-sinPyr[PYR_PITCH]);
    xfm[XYZ_Z][XYZ_Y] = (sinPyr[PYR_ROLL] * cosPyr[PYR_PITCH]);
    xfm[XYZ_Z][XYZ_Z] = (cosPyr[PYR_ROLL] * cosPyr[PYR_PITCH]);
}

void schneider::XYZ(const TX x, const TY y, const TZ z, TXYZ& xyz) noexcept
{
    xyz[XYZ_X] = x;
    xyz[XYZ_Y] = y;
    xyz[XYZ_Z] = z;
}

void schneider::ZauberonInitialize(const TLength xStep, const TXYZ& xyzInitial, TZauberon& zauberon) noexcept
{
    TWaveLength waveLength{RandomWaveLength()};

    zauberon.angle = RandomAngle();
    zauberon.angleStep = AngleStep(xStep, waveLength);
    RandomPitchYawRoll(zauberon.pyr);
    zauberon.radius = Radius(waveLength);
    zauberon.rotation = RandomRotation();
    XFormMatrix(zauberon.pyr, zauberon.xfm);
    zauberon.xyz = xyzInitial;
}

void schneider::ZauberonNewPosition(const TLength xStep, TZauberon& z) noexcept
{
    TXYZ xyzOriginAlongXAxis;
    TXYZ xyzOriginAlongHelixAxis;
    TXYZ xyzNew;

    auto angleNew{(z.rotation == R_RIGHT) ? (z.angle + z.angleStep) : (z.angle - z.angleStep)};
    XYZ(xStep, (z.radius * std::cos(angleNew)), (z.radius * std::sin(angleNew)), xyzOriginAlongXAxis);
    Rotate3D(z.xfm, xyzOriginAlongXAxis, xyzOriginAlongHelixAxis);
    Add(z.xyz, xyzOriginAlongHelixAxis, xyzOriginAlongHelixAxis);
    Map([] UNARY_FN(std::trunc), xyzOriginAlongHelixAxis, xyzNew);
    XYZSpaceAdjustments(xyzNew);
    z.angle = angleNew;
    z.xyz = xyzNew;
}
