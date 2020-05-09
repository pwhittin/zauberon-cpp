#include "../schneider.h"
#include "catch.hpp"

using namespace schneider;

TEST_CASE("AngleStep", "[AngleStepTest]")
{
    CHECK(AngleStep(1, 1) == Approx(0.0));
}

TEST_CASE("PitchYawRoll", "[PitchYawRollTest]")
{
    TPitchYawRoll pyr;
    PitchYawRoll(pyr, 1.0, 2.0, 3.0);
    CHECK(pyr[PYR_PITCH] == Approx(1.0));
    CHECK(pyr[PYR_YAW] == Approx(2.0));
    CHECK(pyr[PYR_ROLL] == Approx(3.0));
    CHECK(pyr.size() == 3);
}

TEST_CASE("Rotate3D", "[Rotate3DTest]")
{
    TPitchYawRoll pyr;
    TXFormMatrix xfm;
    TXYZ xyz;
    TXYZ xyzRotated;
    Rotate3D(xyzRotated, XFormMatrix(xfm, PitchYawRoll(pyr, 1.0, 2.0, 3.0)), XYZ(xyz, 11.0, 22.0, 33.0));
    CHECK(xyzRotated == TXYZ{31.91859139897832, -6.215829627033283, -25.230279133469022});
}

TEST_CASE("SchneiderRadius", "[SchneiderRadiusTest]")
{
    CHECK(SchneiderRadius(1) == Approx(1.35639251216973e+50));
    CHECK(SchneiderRadius(405e+12) == Approx(3.34911731399933E+35));
}

TEST_CASE("XFormMatrix", "[XFormMatrixTest]")
{
    TPitchYawRoll pyr;
    TXFormMatrix xfm;
    XFormMatrix(xfm, PitchYawRoll(pyr, 1.0, 2.0, 3.0));
    CHECK(xfm.size() == 3);
    CHECK(xfm[0].size() == 3);
    CHECK(xfm[1].size() == 3);
    CHECK(xfm[2].size() == 3);
    CHECK(xfm == TXFormMatrix{TXYZ{-0.2248450953661529, 0.8507808619938085, 0.47499116618612774},
                              TXYZ{0.49129549643388193, 0.5199598530948528, -0.698763541087659},
                              TXYZ{-0.8414709848078965, 0.07624746575887673, -0.5348952287053772}});
}

TEST_CASE("XYZ", "[XYZTest]")
{
    TXYZ xyz;
    XYZ(xyz, 1.0, 2.0, 3.0);
    CHECK(xyz[XYZ_X] == Approx(1.0));
    CHECK(xyz[XYZ_Y] == Approx(2.0));
    CHECK(xyz[XYZ_Z] == Approx(3.0));
    CHECK(xyz.size() == 3);
}
