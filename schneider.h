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

using TXYZ = TArray(TNumber, 3);
using TPitchYawRoll = TXYZ;
using TXFormMatrix = TArray(TXYZ, 3);

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

TRadians AngleStep(TLength xStep, TWaveLength waveLength) noexcept;
TPitchYawRoll& PitchYawRoll(TPitchYawRoll& pyr, TPitch x, TYaw y, TRoll z) noexcept;
TXYZ& Rotate3D(TXYZ& xyzRotated, TXFormMatrix& xfm, TXYZ& xyz) noexcept;
TNumber SchneiderRadius(TFrequency f) noexcept;
TXFormMatrix& XFormMatrix(TXFormMatrix& xfm, const TPitchYawRoll& pyr) noexcept;
TXYZ& XYZ(TXYZ& xyzArray, TX x, TY y, TZ z) noexcept;
TZauberon& ZauberonInitialize(TZauberon& zabueron, TLength xStep, const TXYZ& xyzInitial) noexcept;
TZauberon& ZauberonNewPosition(TZauberon& zauberon, TLength xStep) noexcept;

} // namespace schneider

#endif // ZAUBERON_SCHNEIDER_H
