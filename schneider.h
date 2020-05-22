#ifndef ZAUBERON_SCHNEIDER_H
#define ZAUBERON_SCHNEIDER_H

#include "global-types.h"
// TODO: When doing collisions pre-calculate the two PitchYarRoll Xform matrices
/***********************************************************************************************************************
 *  Given
 *      e = energy
 *      H = Plank's constant
 *      C = speed of light
 *      m = mass
 *      f = Zauberon circular helical frequency
 *      r = Zauberon circular helical radius
 *      z = Zauberon mass
 *      S = Schneider constant
 *  and
 *      e = (H * f)
 *      e = (m * (C * C))
 *      z = (S / r)
 *      r = (C / (4 * f))
 *  thus
 *      (H * f)     = (m * (C * C))
 *      (H * f)     = ((S / r) * (C * C))
 *      (H * f)     = ((S * C * C) / r)
 *      (H * f * r) = (S * C * C)
 *      r           = ((S * C * C) / (H * f))
 *      S           = ((H * r * f) / (C * C))
 *      S           = ((H * (C / (4 * f)) * f) / (C * C))
 *  hence
 *      r = (((S * C * C) / H) / f)
 *      S = (H / (4 * C))
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
    TXYZ xyzGrid;
    TXYZ xyzInitial;
};

TRadians AngleStep(TLength xStep, TWaveLength waveLength) noexcept;
TPitchYawRoll& PitchYawRoll(TPitchYawRoll& pyr, TPitch x, TYaw y, TRoll z) noexcept;
TXYZ& Rotate3D(TXYZ& xyzRotated, TXFormMatrix& xfm, TXYZ& xyz) noexcept;
TNumber SchneiderRadius(TFrequency f) noexcept;
TXFormMatrix& XFormMatrix(TXFormMatrix& xfm, const TPitchYawRoll& pyr) noexcept;
TXYZ& XYZ(TXYZ& xyzArray, TX x, TY y, TZ z) noexcept;
TZauberon& ZauberonInitialize(TZauberon& zabueron) noexcept;
TZauberon& ZauberonNewPosition(TZauberon& zauberon) noexcept;

} // namespace schneider

#endif // ZAUBERON_SCHNEIDER_H
