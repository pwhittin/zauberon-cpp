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
static const TNumber SCHNEIDER_CONSTANT{H / (4 * C)};
static const TNumber SCHNEIDER_RADIUS_FREQUENCY_RATIO{(SCHNEIDER_CONSTANT * C * C) / H};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// internal
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const TNumber FREQ_MULTIPLIER{1e+12};
static const int FREQ_MIN{405};
static const int FREQ_MAX{791};
static const int FREQ_COUNT{FREQ_MAX - FREQ_MIN};

TNumbers(FREQ_COUNT) Frequencies;
TNumbers(FREQ_COUNT) WaveLengths;
TNumbers(FREQ_COUNT) Radii;
TLength XStep;
TLength GridStep;
TXYZ XYZInitial{0.0, 0.0, 0.0};
TXYZ XYZGridInitial{0.0, 0.0, 0.0};

static TNumber WaveLength(TFrequency f) noexcept;
static int Random() noexcept;
static bool Initialize() noexcept
{
    Range(Frequencies, FREQ_MIN);
    Multiply(Frequencies, FREQ_MULTIPLIER, Frequencies);
    Map(WaveLengths, WaveLength, Frequencies);
    Map(Radii, SchneiderRadius, Frequencies);
    XStep = WaveLengths[0] / 100.0;
    GridStep = XStep / 10.0;

    std::srand(std::time(0));

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
    return ((Random() % 2) == 0) ? RIGHT : LEFT;
}

static void RandomPitchYawRoll(TPitchYawRoll& pyr) noexcept
{
    PitchYawRoll(pyr, RandomAngle(), RandomAngle(), RandomAngle());
}

static TWaveLength RandomWaveLength() noexcept
{
    return WaveLengths[Random() % WaveLengths.size()];
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

TPitchYawRoll&
schneider::PitchYawRoll(TPitchYawRoll& pyr, const TPitch pitch, const TYaw yaw, const TRoll roll) noexcept
{
    pyr[PITCH] = pitch;
    pyr[YAW] = yaw;
    pyr[ROLL] = roll;
    return pyr;
}

TXYZ& schneider::Rotate3D(TXYZ& xyzRotated, TXFormMatrix& xfm, TXYZ& xyz) noexcept
{
    TXYZ xyzTemp;
    auto MultiplyAdd = [&xyzTemp, &xyzRotated, xfm, xyz](const TXYZEnum X_Y_OR_Z) {
        Multiply(xyzTemp, xfm[X_Y_OR_Z], xyz);
        Add(xyzRotated[X_Y_OR_Z], xyzTemp);
    };

    MultiplyAdd(X);
    MultiplyAdd(Y);
    MultiplyAdd(Z);

    return xyzRotated;
}

TNumber schneider::SchneiderRadius(const TFrequency frequency) noexcept
{
    return SCHNEIDER_RADIUS_FREQUENCY_RATIO / frequency;
}

TXFormMatrix& schneider::XFormMatrix(TXFormMatrix& xfm, const TPitchYawRoll& pyr) noexcept
{
    TXYZ cosPyr;
    Map(cosPyr, [] UNARY_FN(std::cos), pyr);

    TXYZ sinPyr;
    Map(sinPyr, [] UNARY_FN(std::sin), pyr);

    xfm[X][X] = (cosPyr[PITCH] * cosPyr[YAW]);
    xfm[X][Y] = ((sinPyr[ROLL] * sinPyr[PITCH] * cosPyr[YAW]) - (cosPyr[ROLL] * sinPyr[YAW]));
    xfm[X][Z] = ((sinPyr[ROLL] * sinPyr[YAW]) + (cosPyr[ROLL] * sinPyr[PITCH] * cosPyr[YAW]));

    xfm[Y][X] = (cosPyr[PITCH] * sinPyr[YAW]);
    xfm[Y][Y] = ((cosPyr[ROLL] * cosPyr[YAW]) + (sinPyr[ROLL] * sinPyr[PITCH] * sinPyr[YAW]));
    xfm[Y][Z] = ((cosPyr[ROLL] * sinPyr[PITCH] * sinPyr[YAW]) - (sinPyr[ROLL] * cosPyr[YAW]));

    xfm[Z][X] = (-sinPyr[PITCH]);
    xfm[Z][Y] = (sinPyr[ROLL] * cosPyr[PITCH]);
    xfm[Z][Z] = (cosPyr[ROLL] * cosPyr[PITCH]);

    return xfm;
}

TXYZ& schneider::XYZ(TXYZ& xyz, const TX x, const TY y, const TZ z) noexcept
{
    xyz[X] = x;
    xyz[Y] = y;
    xyz[Z] = z;
    return xyz;
}

TZauberon& schneider::ZauberonInitialize(TZauberon& zauberon) noexcept
{
    TWaveLength waveLength{RandomWaveLength()};

    zauberon.angle = RandomAngle();
    zauberon.angleStep = AngleStep(XStep, waveLength);
    RandomPitchYawRoll(zauberon.pyr);
    zauberon.radius = Radius(waveLength);
    zauberon.rotation = RandomRotation();
    XFormMatrix(zauberon.xfm, zauberon.pyr);
    zauberon.xyz = XYZInitial;
    zauberon.xyzGrid = XYZGridInitial;
    zauberon.xyzInitial = XYZInitial;
    return zauberon;
}

TZauberon& schneider::ZauberonNewPosition(TZauberon& z) noexcept
{
    z.angle = (z.rotation == RIGHT) ? (z.angle + z.angleStep) : (z.angle - z.angleStep);

    TXYZ xyzOriginAlongXAxis;
    XYZ(xyzOriginAlongXAxis, XStep, (z.radius * std::cos(z.angle)), (z.radius * std::sin(z.angle)));

    TXYZ xyzOriginAlongHelixAxis;
    Rotate3D(xyzOriginAlongHelixAxis, z.xfm, xyzOriginAlongXAxis);

    Add(z.xyz, z.xyz, xyzOriginAlongHelixAxis);

    TXYZ xyzDelta;
    Subtract(xyzDelta, z.xyz, z.xyzInitial);

    TLength gridStep{GridStep};
    Map(z.xyzGrid, [gridStep](const TNumber delta) { return std::trunc(delta / gridStep); }, xyzDelta);

    return z;
}
