#ifndef ZAUBERON_SCHNEIDER_H
#define ZAUBERON_SCHNEIDER_H

#include "global-types.h"
// TODO: When doing collisions pre-calculate the two PitchYarRoll Xform matrices
/***********************************************************************************************************************
 *  Given
 *      e = energy
 *      h = Plank's constant
 *      c = speed of light
 *      m = mass
 *      f = Zauberon circular helical frequency
 *      r = Zauberon circular helical radius
 *      z = Zauberon mass
 *      s = Schneider constant
 *  then
 *      e = (h * f)
 *      e = (m * (c * c))
 *      z = (s / r)
 *  thus
 *      (h * f)     = (m * (c * c))
 *      (h * f)     = ((s * c * c) / r)
 *      (h * f * r) = (s * c * c)
 *      r           = ((s * c * c) / (h * f))
 *  hence
 *      r = ((s * c * c) / h) / f))
 ***********************************************************************************************************************/
namespace schneider
{

using TDegrees = TNumber;
using TFrequency = TNumber;
using TLength = TNumber;
using TPitch = TNumber;
using TRadians = TNumber;
using TRadius = TNumber;
using TRoll = TNumber;
using TWaveLength = TNumber;
using TX = TNumber;
using TY = TNumber;
using TYaw = TNumber;
using TZ = TNumber;

using TPitchYawRoll = TNumbers;
using TXFormMatrix = std::vector<TNumbers>;
using TXYZ = TNumbers;

enum TRotationEnum
{
    R_LEFT,
    R_RIGHT
};

enum TPitchYawRollEnum
{
    PYR_PITCH,
    PYR_YAW,
    PYR_ROLL
};

enum TXYZEnum
{
    XYZ_X,
    XYZ_Y,
    XYZ_Z
};

struct TZauberon
{
    TRadians angle;
    TRadians angleStep;
    TPitchYawRoll pyr;
    TLength radius;
    TRotationEnum rotation;
    TXFormMatrix xfm;
    TXYZ xyz;
};

using TZauberons = std::vector<TZauberon>;

TRadians AngleStep(TLength xStep, TWaveLength waveLength) noexcept;
TPitchYawRoll PitchYawRoll(TPitch x = 0.0, TYaw y = 0.0, TRoll z = 0.0) noexcept;
TXYZ Rotate3D(const TXFormMatrix& xfm, const TXYZ& xyz) noexcept;
TNumber SchneiderRadius(TFrequency f) noexcept;
TXFormMatrix XFormMatrix(const TPitchYawRoll& pyr) noexcept;
TXYZ XYZ(TX x = 0.0, TY y = 0.0, TZ z = 0.0) noexcept;
TZauberon ZauberonInitialize(TLength xStep, const TXYZ& xyzInitial) noexcept;
void ZauberonNewPosition(TLength xStep, TZauberon& zabueron) noexcept;
TZauberons Zauberons(TIndex numberOfZauberons) noexcept;

} // namespace schneider

#endif // ZAUBERON_SCHNEIDER_H
