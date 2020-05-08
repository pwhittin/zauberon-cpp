#include "../schneider.h"
#include "catch.hpp"

using namespace schneider;
// TODO: Fix all unit tests.  Consider moving the destination to the first parameter of all array functions
TEST_CASE("AngleStep", "[AngleStepTest]")
{
    CHECK(AngleStep(1, 1) == Approx(0.0));
}

TEST_CASE("PitchYawRoll", "[PitchYawRollTest]")
{
    TPitchYawRoll pyr{PitchYawRoll(1.0, 2.0, 3.0)};
    CHECK(pyr[PYR_PITCH] == Approx(1.0));
    CHECK(pyr[PYR_YAW] == Approx(2.0));
    CHECK(pyr[PYR_ROLL] == Approx(3.0));
    CHECK(pyr.size() == 3);
}

TEST_CASE("Rotate3D", "[Rotate3DTest]")
{
    TPitchYawRoll pyr{PitchYawRoll(1.0, 2.0, 3.0)};
    TXFormMatrix xfm{XFormMatrix(pyr)};
    TXYZ xyz{11.0, 22.0, 33.0};
    TXYZ xyzRotated{Rotate3D(xfm, xyz)};
    CHECK(xyzRotated == TXYZ{31.91859139897832, -6.215829627033283, -25.230279133469022});
}

TEST_CASE("SchneiderRadius", "[SchneiderRadiusTest]")
{
    CHECK(SchneiderRadius(1) == Approx(2.7978344950466395e-30));
    CHECK(SchneiderRadius(405e+12) == Approx(1.13312e-15));
}

TEST_CASE("XFormMatrix", "[XFormMatrixTest]")
{
    TPitchYawRoll pyr{PitchYawRoll(1.0, 2.0, 3.0)};
    TXFormMatrix xfm{XFormMatrix(pyr)};
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
    TXYZ xyz{XYZ(1.0, 2.0, 3.0)};
    CHECK(xyz[XYZ_X] == Approx(1.0));
    CHECK(xyz[XYZ_Y] == Approx(2.0));
    CHECK(xyz[XYZ_Z] == Approx(3.0));
    CHECK(xyz.size() == 3);
}
